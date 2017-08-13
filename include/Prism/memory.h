#ifndef H_PRISM_MEMORY_INCLUDED
#define H_PRISM_MEMORY_INCLUDED

#include <Prism/config.h>
#include <Prism/types.h>

PR_CPP_PROTECT_ON

    PR_STRUCT(pr_memorypool_t, Pr_MemoryPool);

    extern PRISM_API Pr_MemoryPool *    Pr_NewMemoryPool(Pr_MemoryPool *, pr_u32_t, pr_u8_t);
    extern PRISM_API void               Pr_DeleteMemoryPool(Pr_MemoryPool *);
    extern PRISM_API void *             Pr_AllocateBlock(Pr_MemoryPool *);
    extern PRISM_API void               Pr_FreeBlock(Pr_MemoryPool *, void *);
    
    PR_STRUCT(pr_array_t, Pr_Array);

    typedef pr_bool_t   (*Pr_Initializer)(void *, pr_u32_t);
    typedef void        (*Pr_Deleter)(void *);

    extern PRISM_API Pr_Array * Pr_NewArray(pr_u32_t stride, Pr_Initializer);
    extern PRISM_API void       Pr_DeleteArray(Pr_Array *);
    extern PRISM_API void *     Pr_SetArrayAt(Pr_Array *, pr_u32_t, void *);
    extern PRISM_API pr_u32_t   Pr_ArraySize(Pr_Array *);
    extern PRISM_API void *     Pr_GetArrayData(Pr_Array *);
    extern PRISM_API pr_bool_t  Pr_ResizeArray(Pr_Array *, pr_u32_t);

    typedef Pr_Array Pr_BitSet;


PR_CPP_PROTECT_OFF

#endif /* H_PRISM_MEMORY_INCLUDED */


