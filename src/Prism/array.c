#include <Prism/array.h>
#include <stdlib.h>

struct pr_array_t {
    void *      ptr;
    pr_u32_t    size;
    pr_u32_t    capacity;
    pr_u32_t    chunk; 
};

static pr_bool_t s_Pr_ExtendArray(Pr_Array * ap_array, pr_u32_t a_size)
{
    void *      lp_tmp;
    pr_u32_t    l_cap;

    l_cap = (ap_array->capacity > 0) ? ap_array->capacity : 1;

    while (l_cap <= a_size) {
        l_cap <<= 1;
    }

    if (!ap_array->ptr) {
        lp_tmp = malloc(l_cap * ap_array->chunk);
    } else {
        lp_tmp = realloc(ap_array->ptr, l_cap * ap_array->chunk);
    }

    if (!lp_tmp) return PR_FALSE;

    ap_array->ptr       = lp_tmp;
    ap_array->capacity  = l_cap;
    ap_array->size      = a_size;

    return PR_TRUE;
}

Pr_Array * Pr_NewArray(pr_u32_t a_size, pr_u32_t a_chunk)
{
    Pr_Array * lp_out;

    if (a_chunk == 0) return 0;

    lp_out = malloc(sizeof(Pr_Array));
    if (!lp_out) return NULL;

    lp_out->chunk       = a_chunk;
    lp_out->size        = 0;
    lp_out->capacity    = 0;
    lp_out->ptr         = NULL;

    if (!s_Pr_ExtendArray(lp_out, a_size)) {
        free(lp_out);
        return NULL;
    }

    return lp_out;
}

void Pr_DeleteArray(Pr_Array * ap_array)
{
    if (!ap_array) return;

    if (ap_array->ptr) {
        free(ap_array->ptr);
    }

    free(ap_array);
}

pr_u32_t Pr_ArraySize(Pr_Array * ap_array)
{
    return (ap_array) ? ap_array->size : 0;
}

pr_u32_t Pr_ArrayCapacity(Pr_Array * ap_array)
{
    return (ap_array) ? ap_array->capacity : 0;
}

void * const Pr_GetArrayPointer(Pr_Array * ap_array)
{
    return (ap_array) ? ap_array->ptr : NULL;
}

pr_bool_t Pr_ResizeArray(Pr_Array * ap_array, pr_u32_t a_size)
{
    if (!ap_array) return PR_FALSE;

    if (a_size < ap_array->capacity) {
        ap_array->size = a_size;
        return PR_TRUE;
    } 

    return s_Pr_ExtendArray(ap_array, a_size);
}

void Pr_ClearArray(Pr_Array * ap_array)
{
    if (!ap_array) return;

    ap_array->size = 0;
}


