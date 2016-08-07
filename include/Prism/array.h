/* File name : array.h
 * File type : Header
 * Author : Alexandre DUBUJET
 * Creation : 2015
 * Desc. : C Array implementation
 * Dependencies :	<stdlib.h>
 */

#ifndef H_PRISM_ARRAY_INCLUDED
#define H_PRISM_ARRAY_INCLUDED

#include <Prism/config.h>
#include <Prism/types.h>

PR_CPP_PROTECT_ON

    PR_STRUCT(pr_array_t, Pr_Array);
    
    Pr_Array *      Pr_NewArray(pr_u32_t, pr_u32_t);
    void            Pr_DeleteArray(Pr_Array *);
    pr_u32_t        Pr_ArraySize(Pr_ArrayRef);
    pr_u32_t        Pr_ArrayCapacity(Pr_ArrayRef);
    void * const    Pr_GetArrayPointer(Pr_ArrayRef);
    pr_bool_t       Pr_ResizeArray(Pr_Array *, pr_u32_t);
    void            Pr_ClearArray(Pr_Array *);

PR_CPP_PROTECT_OFF

#endif

