/* File name : list.c
 * File type : Source
 * Author : Alexandre DUBUJET
 * Creation : 2013
 * Desc. : C Linked list implementation
 * Dependencies :	<stdlib.h>, 
					<Prism/list.h>
 */

#include <stdlib.h>
#include <Prism/list.h>

typedef struct pr_node_t pr_node_t;
struct pr_node_t {
    pr_node_t * next;
    pr_node_t * prev;
    void *      data;
    Pr_List *   owner;
};

struct pr_list_t {
    pr_node_t * first;
    pr_node_t * last;
    pr_u32_t    size;
};

static pr_node_t * s_Pr_NewNode(Pr_List * ap_list, void * ap_data)
{
    pr_node_t * lp_out = malloc(sizeof(pr_node_t));

    if (lp_out) {
        lp_out->data = ap_data;
        lp_out->next = NULL;
        lp_out->prev = NULL;
        lp_out->owner = ap_list;
    }

    return lp_out;
}

Pr_List *       Pr_NewList(void)
{
    Pr_List * lp_out = malloc(sizeof(Pr_List));

    if (lp_out) {
        lp_out->first = NULL;
        lp_out->size = 0;
        lp_out->last = NULL;
    }

    return lp_out;
}

void            Pr_DeleteList(Pr_List * ap_list)
{
    if (ap_list) {
        Pr_ClearList(ap_list);
        free(ap_list);
    }
}

void            Pr_ClearList(Pr_List * ap_list)
{
    if (ap_list) {
        while (ap_list->first) {
            Pr_EraseListElement(ap_list, Pr_ListBegin(ap_list));
        }
    }
}

Pr_ListIterator Pr_PushBackList(Pr_List * ap_list, void * ap_data)
{
    Pr_ListIterator lp_out;

    if (!ap_list) return NULL;

    lp_out = s_Pr_NewNode(ap_list, ap_data);
    if (!lp_out) return NULL;

    ap_list->size++;

    if (!ap_list->first) {
        ap_list->first = lp_out;
        ap_list->last = lp_out;
        return lp_out;
    }

    lp_out->prev = ap_list->last;
    ap_list->last->next = lp_out;
    ap_list->last = lp_out;

    return lp_out;
}

Pr_ListIterator Pr_PushFrontList(Pr_List * ap_list, void * ap_data)
{
    Pr_ListIterator lp_out;

    if (!ap_list) return NULL;

    lp_out = s_Pr_NewNode(ap_list, ap_data);
    if (!lp_out) return NULL;

    ap_list->size++;

    if (!ap_list->first) {
        ap_list->first = lp_out;
        ap_list->last = lp_out;
        return lp_out;
    }

    ap_list->first->prev = lp_out;
    lp_out->next = ap_list->first;
    ap_list->first = lp_out;

    return lp_out;
}

Pr_ListIterator Pr_EraseListElement(Pr_List * ap_list, Pr_ListIterator ap_it)
{
    Pr_ListIterator lp_out;

    if (!ap_list || !ap_it) return NULL;

    if (ap_it->owner != ap_list) return NULL;

    lp_out = ap_it->next;

    if (ap_it == ap_it->owner->first) {
        ap_it->owner->first = lp_out;
        if (lp_out) {
            lp_out->prev = NULL;
        } else {
            ap_it->owner->last = NULL;
        }
    } else {
        ap_it->prev->next = lp_out;
        if (lp_out) {
            lp_out->prev = ap_it->prev;
        }
    }

    ap_it->owner->size--;

    free(ap_it);

    return lp_out;
}

pr_u32_t        Pr_ListSize(Pr_List * ap_list)
{
    return (ap_list) ? ap_list->size : 0;
}

Pr_ListIterator Pr_ListBegin(Pr_List * ap_list)
{
    return ap_list ? ap_list->first : NULL;
}

Pr_ListIterator Pr_ListBack(Pr_List * ap_list)
{
    return ap_list ? ap_list->last : NULL;
}

Pr_ListIterator Pr_NextListIterator(Pr_ListIterator ap_it)
{
    return ap_it ? ap_it->next : NULL;
}

Pr_ListIterator Pr_PreviousListIterator(Pr_ListIterator ap_it)
{
    return ap_it ? ap_it->prev : NULL;
}

void *          Pr_ListIteratorData(Pr_ListIterator ap_it)
{
    return ap_it ? ap_it->data : NULL;
}

void            Pr_SetListIteratorData(Pr_ListIterator ap_it, void * ap_data)
{
	if (ap_it) {
		ap_it->data = ap_data;
	}
}

void            Pr_SwapListElements(Pr_ListIterator ap_1, Pr_ListIterator ap_2)
{
    void * lp_tmp;

    if (!ap_1 && !ap_2) return;

    lp_tmp = ap_1->data;
    ap_1->data = ap_2->data;
    ap_2->data = lp_tmp;
}

Pr_List *       Pr_CopyList(Pr_List * ap_list)
{
    Pr_List * lp_out;
    Pr_ListIterator lp_it;

    if (!ap_list) return NULL;

    lp_out = Pr_NewList();
    if (!lp_out) return NULL;

    lp_out->size = ap_list->size;

    PR_LIST_FOREACH(ap_list, lp_it) {
        if (!Pr_PushBackList(lp_out, Pr_ListIteratorData(lp_it))) {
            Pr_DeleteList(lp_out);
            return NULL;
        }
    }

    return lp_out;
}

static void s_Pr_QSort(Pr_ListIterator ap_from, Pr_ListIterator ap_to, Pr_ListComparator af_comp)
{
    Pr_ListIterator lp_right = ap_to;
    Pr_ListIterator lp_pivot = ap_from;
    Pr_ListIterator lp_left = Pr_NextListIterator(ap_from);

    if (ap_from == ap_to) return;

    while (lp_left != lp_right) {
        if (af_comp(Pr_ListIteratorData(lp_left), Pr_ListIteratorData(lp_pivot))) {
            lp_left = Pr_NextListIterator(lp_left);
        } else {
            while((lp_left != lp_right) 
                && af_comp(Pr_ListIteratorData(lp_pivot), Pr_ListIteratorData(lp_right))
            ) {
                lp_right = Pr_PreviousListIterator(lp_right);
            }

            Pr_SwapListElements(lp_left, lp_right);
        }
    }

    lp_left = Pr_PreviousListIterator(lp_left);

    Pr_SwapListElements(lp_pivot, lp_left);

    s_Pr_QSort(ap_from, lp_left, af_comp);
    s_Pr_QSort(lp_right, ap_to, af_comp);
}

void Pr_SortList(Pr_ListIterator ap_from, Pr_ListIterator ap_to, Pr_ListComparator af_comp)
{
    if (!ap_from || !ap_to || !af_comp) return;

    if (ap_from->owner != ap_to->owner) return;

    s_Pr_QSort(ap_from, ap_to, af_comp);
}



