#ifndef H_PRISM_MEMORY_INCLUDED
#define H_PRISM_MEMORY_INCLUDED

#include <Prism/config.h>
#include <Prism/types.h>

PR_CPP_PROTECT_ON

    PR_STRUCT(pr_memorypool_t, Pr_MemoryPool);

    Pr_MemoryPool * Pr_NewMemoryPool(Pr_MemoryPool *, pr_u32_t, pr_u8_t);
    void            Pr_DeleteMemoryPool(Pr_MemoryPool *);
    void *          Pr_AllocateBlock(Pr_MemoryPool *);
    void            Pr_FreeBlock(Pr_MemoryPool *, void *);
    
    PR_STRUCT(pr_array_t, Pr_Array);

    typedef pr_bool_t (*Pr_Initializer)(void *, pr_u32_t);

    Pr_Array *      Pr_NewArray(pr_u32_t stride, Pr_Initializer);
    void            Pr_DeleteArray(Pr_Array *);
    pr_bool_t       Pr_SetArrayAt(Pr_Array *, pr_u32_t, void *);
    pr_u32_t        Pr_ArraySize(Pr_Array *);
    void * const    Pr_GetArrayData(Pr_Array *);

    typedef Pr_Array Pr_BitSet;

    Pr_BitSet *     Pr_NewBitSet();
    pr_bool_t       Pr_SetBitSetAt(Pr_BitSet *, pr_u32_t a_at, char);

PR_CPP_PROTECT_OFF

#endif /* H_PRISM_MEMORY_INCLUDED */


