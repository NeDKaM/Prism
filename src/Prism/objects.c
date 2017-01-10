#include <Prism/objects.h>
#include <stdlib.h>
#include <stdio.h>

Pr_Class Pr_ObjectClass = {
    /* name */          "Pr_ObjectClass",
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
            if (!lp_cls->construct(ap_obj)) {
                l_err++;
                break;
            }
        }

        l_counter--;
        lp_cls = ap_obj->class;
    }

    if (lp_cls->construct) {
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

    if (lp_out->onDelete) {
        if (s_Pr_InheritedConstructs(lp_out)) {
            s_Pr_UpdateClassRefCounters(ap_cls, 1);
            return lp_out;
        }

        Pr_DeleteSignal(lp_out->onDelete);
    }

    Pr_FreeBlock(ap_cls->memoryPool, lp_out);

    return NULL;
}

void Pr_Delete(Pr_ObjectRef ap_obj)
{
    Pr_Class * lp_cls = NULL;

    if (!ap_obj) return;

    lp_cls = ap_obj->class;

    Pr_Emit(ap_obj->onDelete);

    s_Pr_InheritedDestructs(ap_obj);

    Pr_FreeBlock(lp_cls->memoryPool, ap_obj);

    s_Pr_UpdateClassRefCounters(lp_cls, -1);

    if (lp_cls->refCount == 0) {
        Pr_DeleteMemoryPool(lp_cls->memoryPool);
        lp_cls->memoryPool = NULL;
    }
}

