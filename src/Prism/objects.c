#include <Prism/objects.h>

#include <Prism/table.h>
#include <Prism/string.h>

#include <stdlib.h>
#include <stdio.h>

Pr_Class Pr_ObjectClass = {
    /* name */          "Pr_Object",
    /* objectSize */    sizeof(Pr_Object),
    /* refCount */      0,
    /* inherit */       NULL,
    /* memoryPool */    NULL,
    /* poolSize */      0,
    /* abstract */      PR_TRUE,

    NULL,
    NULL
};

static void s_Pr_UpdateClassRefCounters(Pr_Class * ap_cls, long a_value)
{
    Pr_Class * lp_tmp = ap_cls;
        
    while (lp_tmp) {
        lp_tmp->refCount += a_value;
        lp_tmp = lp_tmp->inherit;
    }    
}

static pr_bool_t s_Pr_InheritedConstructs(Pr_ObjectRef ap_obj)
{
    Pr_Class * lp_cls = ap_obj->class;
    pr_u32_t l_err = 0;
    pr_u32_t l_counter = 0;

    while (lp_cls->inherit) {
        lp_cls = lp_cls->inherit;
        l_counter++;
    }

    lp_cls = ap_obj->class;

    while (l_counter) {
        pr_u32_t l_i;
        for (l_i = 0 ; l_i < l_counter ; l_i++) {
            lp_cls = lp_cls->inherit;
        }

        if (lp_cls->construct) {
            Pr_Emit(ap_obj->onConstruction, ap_obj, lp_cls);
            if (!lp_cls->construct(ap_obj)) {
                l_err++;
                break;
            } 
        }

        l_counter--;
        lp_cls = ap_obj->class;
    }

    if (lp_cls->construct) {
        Pr_Emit(ap_obj->onConstruction, ap_obj, lp_cls);
        if (!lp_cls->construct(ap_obj)) {
            l_err++;
        } 
    }

    return (l_err) ? PR_FALSE : PR_TRUE; 
}

static void s_Pr_InheritedDestructs(Pr_ObjectRef ap_obj)
{		
    Pr_Class * lp_cls = ap_obj->class;

    if (lp_cls->destruct) {
        lp_cls->destruct(ap_obj);
    }

    while (lp_cls->inherit) {
        lp_cls = lp_cls->inherit;
        if (lp_cls->destruct) {
            lp_cls->destruct(ap_obj);
        }
    }
}

void * Pr_New(Pr_Class * ap_cls)
{
    Pr_ObjectRef lp_out;

    lp_out = Pr_Alloc(ap_cls);
    if (lp_out) {
        if (Pr_Construct(lp_out)) {
            return lp_out;
        } 

        Pr_Delete(lp_out);
    }

    return NULL;
}

void * Pr_Alloc(Pr_Class * ap_cls)
{
    Pr_ObjectRef lp_out = NULL;

    if (!ap_cls) return NULL;
    
    if (ap_cls->abstract) return NULL;

    if (!ap_cls->memoryPool) {
        ap_cls->poolSize = (ap_cls->poolSize) ? ap_cls->poolSize : 16;
        ap_cls->memoryPool = Pr_NewMemoryPool(
            NULL, 
            ap_cls->objectSize, 
            ap_cls->poolSize
        );

        if (!ap_cls->memoryPool) return NULL;
    }

    lp_out = Pr_AllocateBlock(ap_cls->memoryPool);
    if (!lp_out) return NULL;

    lp_out->class = ap_cls;
    lp_out->onDelete = Pr_NewSignal();
    lp_out->onConstruction = Pr_NewSignal();

    if (lp_out->onDelete && lp_out->onConstruction) {
        s_Pr_UpdateClassRefCounters(ap_cls, 1);
        return lp_out;
    }

    Pr_Delete(lp_out);

    return NULL;
}

void Pr_Delete(Pr_ObjectRef ap_obj)
{
    Pr_Class * lp_cls = NULL;

    if (!ap_obj) return;

    lp_cls = ap_obj->class;

    Pr_Emit(ap_obj->onDelete);

    s_Pr_InheritedDestructs(ap_obj);

    Pr_DeleteSignal(ap_obj->onConstruction);
    Pr_DeleteSignal(ap_obj->onDelete);

    Pr_FreeBlock(lp_cls->memoryPool, ap_obj);

    s_Pr_UpdateClassRefCounters(lp_cls, -1);

    if (lp_cls->refCount == 0) {
        Pr_DeleteMemoryPool(lp_cls->memoryPool);
        lp_cls->memoryPool = NULL;
    }
}

pr_bool_t  Pr_Construct(Pr_ObjectRef ap_obj)
{
    if (!ap_obj) return PR_FALSE;

    if (s_Pr_InheritedConstructs(ap_obj)) {
        return PR_TRUE;
    }

    s_Pr_InheritedDestructs(ap_obj);
    return PR_FALSE;
}

struct pr_objectlibrary_t {
    Pr_String * name;
    Pr_Table *  objects;
};

Pr_ObjectLibrary *  Pr_NewObjectLibrary(pr_cstring_t const ap_name)
{
    Pr_ObjectLibrary * lp_out;

    lp_out = malloc(sizeof(Pr_ObjectLibrary));
    if (!lp_out) return NULL;

    lp_out->name = Pr_NewString();
    lp_out->objects = Pr_NewTable(NULL);

    if (lp_out->name && lp_out->objects) {
        Pr_SetStringStr(lp_out->name, (ap_name && ap_name[0]) ? ap_name : "UnnamedLibrary");
        return lp_out;
    }

    Pr_DeleteObjectLibrary(lp_out);

    return NULL;
}

void                Pr_DeleteObjectLibrary(Pr_ObjectLibrary * ap_lib)
{
    if (!ap_lib) return;

    Pr_DeleteString(ap_lib->name);
    Pr_DeleteTable(ap_lib->objects);

    free(ap_lib);
}

pr_bool_t           Pr_RegisterObject(Pr_ObjectLibrary * ap_lib, Pr_ObjectRef ap_obj, pr_cstring_t const ap_str)
{
    Pr_ListIterator lp_it;

    if (!ap_lib || !ap_obj || !ap_str) return PR_FALSE;

    lp_it = Pr_GetTableIterator(ap_lib->objects, PR_TKEY_STR(ap_str));
    if (lp_it) {
        Pr_SetListIteratorData(lp_it, ap_obj);
        return PR_TRUE;
    }

    return PR_FALSE;
}

void                Pr_UnregisterObject(Pr_ObjectLibrary * ap_lib, pr_cstring_t const ap_str)
{
    if (!ap_lib || !ap_str) return;

    Pr_EraseTableElement(ap_lib->objects, PR_TKEY_STR(ap_str));
}

Pr_ObjectRef        Pr_GetObject(Pr_ObjectLibrary * ap_lib, pr_cstring_t const ap_str)
{
    if (!ap_lib || !ap_str) return NULL;

    if (Pr_TableElementExists(ap_lib->objects, PR_TKEY_STR(ap_str))) {
        return Pr_ListIteratorData(Pr_GetTableIterator(ap_lib->objects, PR_TKEY_STR(ap_str)));
    }

    return NULL;
}

pr_bool_t Pr_IsBaseOf(Pr_Class * ap_class, Pr_Class * ap_inherit)
{
    Pr_Class * lp_tmp;

    if (!ap_class || !ap_inherit) return PR_FALSE;

    lp_tmp = ap_class;

    while (lp_tmp) {
        if (lp_tmp == ap_inherit) return PR_TRUE;
        lp_tmp = lp_tmp->inherit;
    }

    return PR_FALSE;
}

