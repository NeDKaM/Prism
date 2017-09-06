#include <Prism/table.h>
#include <Prism/memory.h>

#include <stdlib.h>
#include <string.h>

typedef struct {
    void *          key;
    pr_u32_t        keySize;
    Pr_ListIterator it;
} Pr_TableSlot;

struct pr_table_t {
    Pr_List *   list;
    Pr_Array *  slots;

    pr_u32_t    (*hash)(void *, pr_u32_t);
};

static pr_u32_t s_Pr_DefaultHash_djb2(void * ap_key, pr_u32_t a_size)
{
    pr_u32_t hash = 5381;
    pr_u32_t l_i;

    for (l_i = 0 ; l_i < a_size ; l_i++) {
        pr_u8_t c = ((pr_u8_t *) ap_key)[l_i];
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

static pr_bool_t s_Pr_SlotInitializer(void * ap_data, pr_u32_t a_)
{
    Pr_TableSlot * lp_slot = ap_data;

    lp_slot->it     = NULL;
    lp_slot->key    = NULL;
    lp_slot->keySize = 0;

    return PR_TRUE;
}

Pr_Table * Pr_NewTable(pr_u32_t (*ap_hash)(void * key, pr_u32_t))
{
    Pr_Table * lp_out;

    lp_out = malloc(sizeof(Pr_Table));
    if (!lp_out) return NULL;

    lp_out->list = Pr_NewList();
    lp_out->slots = Pr_NewArray(sizeof(Pr_TableSlot), s_Pr_SlotInitializer);

    if (lp_out->slots && lp_out->list) {
        if (Pr_ResizeArray(lp_out->slots, 16)) {
            lp_out->hash = (ap_hash) ? ap_hash : s_Pr_DefaultHash_djb2;
            return lp_out;
        }
    }

    Pr_DeleteTable(lp_out);

    return NULL;
}

void       Pr_DeleteTable(Pr_Table * ap_table)
{
    Pr_TableSlot * lp_slots;
    pr_u32_t l_i;

    if (!ap_table) return;

    Pr_DeleteList(ap_table->list);

    lp_slots = Pr_GetArrayData(ap_table->slots);
    for (l_i=0 ; l_i<Pr_ArrayCapacity(ap_table->slots) ; l_i++) {
        free(lp_slots[l_i].key);
    }
    Pr_DeleteArray(ap_table->slots);

    free(ap_table);
}

Pr_ListIterator    Pr_TableBegin(Pr_Table * ap_table)
{
    if (!ap_table) return NULL;

    return Pr_ListBegin(ap_table->list);
}

Pr_ListIterator    Pr_TableBack(Pr_Table * ap_table)
{
    if (!ap_table) return NULL;

    return Pr_ListBack(ap_table->list);
}

static pr_bool_t s_Pr_ReHashTable(Pr_Table * ap_table)
{
    Pr_Array * lp_newSlotsArray;
    pr_u32_t l_cap;

    lp_newSlotsArray = Pr_NewArray(sizeof(Pr_TableSlot), s_Pr_SlotInitializer);
    if (!lp_newSlotsArray) return PR_FALSE;

    l_cap = Pr_ArrayCapacity(ap_table->slots);

    if (Pr_ResizeArray(lp_newSlotsArray, l_cap)) {
        Pr_TableSlot * lp_oldSlots = Pr_GetArrayData(ap_table->slots);
        Pr_TableSlot * lp_newSlots = Pr_GetArrayData(lp_newSlotsArray);
        pr_u32_t l_i;

        for (l_i=0 ; l_i<l_cap ; l_i++) {
            pr_u32_t l_hashCode = ap_table->hash(lp_oldSlots[l_i].key, lp_oldSlots[l_i].keySize) % Pr_ArrayCapacity(lp_newSlotsArray);
            lp_newSlots[l_hashCode] = lp_oldSlots[l_i];
        }

        Pr_DeleteArray(ap_table->slots);
        ap_table->slots = lp_newSlotsArray;

        return PR_TRUE;
    }

    Pr_DeleteArray(lp_newSlotsArray);

    return PR_FALSE;
}

static Pr_ListIterator s_Pr_InsertElement(Pr_Table * ap_table, void * ap_key, pr_u32_t a_keySize, pr_u32_t a_hashCode)
{
    Pr_TableSlot * lp_slots;
    void * lp_key;
    Pr_TableSlot * lp_s;

    lp_key = malloc(a_keySize);
    if (!lp_key) return NULL;

    lp_slots = Pr_GetArrayData(ap_table->slots);

    lp_s = &lp_slots[a_hashCode];

    if (!lp_s->it) {
        lp_s->it = Pr_PushBackList(ap_table->list, NULL);
        if (!lp_s->it) {
            free(lp_key);
            return NULL;
        }
    }

    memcpy(lp_key, ap_key, a_keySize);
    free(lp_s->key);
    lp_s->key = lp_key;
    lp_s->keySize = a_keySize;

    return lp_s->it;
}

Pr_ListIterator Pr_GetTableIterator(Pr_Table * ap_table, void * ap_key, pr_u32_t a_keySize)
{
    pr_u32_t l_hashCode;
    pr_u32_t l_cap;

    if (!ap_table || !ap_key || !a_keySize) return NULL;

    l_cap = Pr_ArrayCapacity(ap_table->slots);

    l_hashCode = ap_table->hash(ap_key, a_keySize) % l_cap;

    if (Pr_TableElementExists(ap_table, ap_key, a_keySize)) {
        Pr_TableSlot * lp_slots = Pr_GetArrayData(ap_table->slots);
        return lp_slots[l_hashCode].it;
    }

    if (Pr_ListSize(ap_table->list) >= (pr_u32_t)(l_cap * .7f)) {
        while (Pr_ListSize(ap_table->list) >= (pr_u32_t)(l_cap * .7f)) {
            l_cap *= 2;
        }

        if (!Pr_ResizeArray(ap_table->slots, l_cap)) return NULL;

        if (!s_Pr_ReHashTable(ap_table)) return NULL;

        l_hashCode = ap_table->hash(ap_key, a_keySize) % Pr_ArrayCapacity(ap_table->slots);
    }

    return s_Pr_InsertElement(ap_table, ap_key, a_keySize, l_hashCode);
}
    
void Pr_EraseTableElement(Pr_Table * ap_table, void * ap_key, pr_u32_t a_keySize)
{
    pr_u32_t l_hashCode;
    Pr_TableSlot * lp_slots;
    Pr_TableSlot * lp_s;

    if (!ap_table || !ap_key || !a_keySize) return;

    l_hashCode = ap_table->hash(ap_key, a_keySize) % Pr_ArrayCapacity(ap_table->slots);

    lp_slots = Pr_GetArrayData(ap_table->slots);

    lp_s = &lp_slots[l_hashCode];

    Pr_EraseListElement(ap_table->list, lp_s->it);

    free(lp_s->key);

    s_Pr_SlotInitializer(lp_s, /* unused */ 0);
}

pr_bool_t Pr_TableElementExists(Pr_Table * ap_table, void * ap_key, pr_u32_t a_keySize)
{
    Pr_TableSlot * lp_slots;
    pr_u32_t l_hashCode;
    Pr_TableSlot * lp_s;

    if (!ap_table || !ap_key || !a_keySize) return PR_FALSE;

    lp_slots = Pr_GetArrayData(ap_table->slots);

    l_hashCode = ap_table->hash(ap_key, a_keySize) % Pr_ArrayCapacity(ap_table->slots);

    lp_s = &lp_slots[l_hashCode];

    if (lp_s->keySize != a_keySize) return PR_FALSE;

    if (memcmp(lp_s->key, ap_key, a_keySize) == 0) {
        return PR_TRUE;
    }

    return PR_FALSE;
}
