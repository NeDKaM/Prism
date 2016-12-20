#ifndef H_PRISM_MEMORY_INCLUDED
#define H_PRISM_MEMORY_INCLUDED

#include <Prism/types.h>

PR_CPP_PROTECT_ON

    typedef struct pr_memorypool_t Pr_MemoryPool;

    Pr_MemoryPool * Pr_NewMemoryPool(Pr_MemoryPool *, pr_u32_t, pr_u8_t);
    void            Pr_DeleteMemoryPool(Pr_MemoryPool *);
    void *          Pr_AllocateBlock(Pr_MemoryPool *);
    void            Pr_FreeBlock(Pr_MemoryPool *, void *);

PR_CPP_PROTECT_OFF

#endif /* H_PRISM_MEMORY_INCLUDED */


