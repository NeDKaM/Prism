#include <Prism/memory.h>
#include <stdlib.h>
#include <stdio.h>

struct pr_memorypool_t {
    pr_u32_t        blockSize;
    Pr_MemoryPool * nextPool;
    Pr_MemoryPool * previousPool;
    void **         freeList;
    pr_u8_t *       chunk;
    pr_u8_t         count;
    pr_u8_t         freeCount;
};

Pr_MemoryPool * Pr_NewMemoryPool(Pr_MemoryPool * ap_prev, pr_u32_t a_blockSize, pr_u8_t a_count)
{
    Pr_MemoryPool * lp_out = NULL;
    pr_u8_t * lp_chunk = NULL;
    void ** lp_freeList = NULL;
    int l_i;

    if (!a_blockSize || !a_count) return NULL;

    lp_chunk = malloc(sizeof(pr_u8_t) * a_blockSize * a_count);
    
    lp_freeList = malloc(sizeof(void *) * a_count);

    if (lp_chunk && lp_freeList) {
        lp_out = malloc(sizeof(Pr_MemoryPool));
        if (lp_out) {
            for (l_i = 0 ; l_i < a_count ; l_i++) {
                lp_freeList[l_i] = &lp_chunk[a_blockSize * (a_count - l_i - 1)];
            }

            lp_out->blockSize       = a_blockSize;
            lp_out->chunk           = lp_chunk;
            lp_out->count           = a_count;
            lp_out->freeList        = lp_freeList;
            lp_out->nextPool        = NULL;
            lp_out->previousPool    = ap_prev;
            lp_out->freeCount       = a_count;           

            return lp_out;
        }
    }

    free(lp_chunk);
    free(lp_freeList);

    return NULL;
}

void Pr_DeleteMemoryPool(Pr_MemoryPool * ap_pool)
{
    if (!ap_pool) return;  

    free(ap_pool->chunk);
    free(ap_pool->freeList);
    Pr_DeleteMemoryPool(ap_pool->nextPool);
    free(ap_pool);
}

void * Pr_AllocateBlock(Pr_MemoryPool * ap_pool)
{
    if (!ap_pool) return NULL;
    if (!ap_pool->chunk) return NULL;

    if (ap_pool->freeCount > 0) {
        return ap_pool->freeList[--ap_pool->freeCount];
    }

    if (!ap_pool->nextPool) {
        ap_pool->nextPool = Pr_NewMemoryPool(ap_pool, ap_pool->blockSize, ap_pool->count * 2);
        if (!ap_pool->nextPool) {
            return malloc(ap_pool->blockSize);
        }
    }

    return Pr_AllocateBlock(ap_pool->nextPool);
}

void Pr_FreeBlock(Pr_MemoryPool * ap_pool, void * ap_ptr)
{
    pr_u8_t * lp_freePtr;
    pr_u8_t * lp_poolChunk;

    if (!ap_pool || !ap_ptr) return;

    lp_freePtr      = ap_ptr;
    lp_poolChunk    = ap_pool->chunk;

    if (lp_freePtr >= lp_poolChunk && 
        lp_freePtr < lp_poolChunk + ap_pool->blockSize * ap_pool->count
    ) {
        ap_pool->freeList[ap_pool->freeCount++] = ap_ptr;
        
        if (ap_pool->freeCount == ap_pool->count 
            && ap_pool->previousPool
            && !ap_pool->nextPool
        ) {
            if (ap_pool->previousPool->freeCount > 0) {
                ap_pool->previousPool->nextPool = NULL;
                Pr_DeleteMemoryPool(ap_pool);
            }
        }    
    } else {
        if (ap_pool->nextPool) {
            Pr_FreeBlock(ap_pool->nextPool, ap_ptr);
        } else {
            free(ap_ptr);
        }
    }
}
