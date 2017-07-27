#include <Prism/memory.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

struct pr_array_t {
    pr_u32_t        size;
    pr_u32_t        capacity;
    pr_u32_t        stride;
    Pr_Initializer  initializer;
    char *          data;
};

static pr_bool_t s_Pr_DefaultInitializer(void * ap_data, pr_u32_t size)
{
    memset(ap_data, 0, size);

    return PR_TRUE;
}

Pr_Array * Pr_NewArray(pr_u32_t a_stride, Pr_Initializer af_init)
{
    Pr_Array * lp_out;

    if (!a_stride) return NULL;

    lp_out = malloc(sizeof(Pr_Array));
    if (!lp_out) return NULL;

    lp_out->data = malloc(a_stride);
    if (!lp_out->data) {
        free(lp_out);
        return NULL;
    }

    lp_out->stride      = a_stride;
    lp_out->size        = 0;
    lp_out->capacity    = 1;
    lp_out->initializer = af_init ? af_init : s_Pr_DefaultInitializer;

    return lp_out;
}

void Pr_DeleteArray(Pr_Array * ap_array)
{
    if (!ap_array) return;

    free(ap_array->data);
    free(ap_array);
}

pr_u32_t Pr_ArraySize(Pr_Array * ap_array)
{
    return ap_array ? ap_array->size : 0;
}

void * Pr_GetArrayData(Pr_Array * ap_array)
{
    return ap_array ? ap_array->data : NULL;
}

pr_bool_t Pr_SetArrayAt(Pr_Array * ap_array, pr_u32_t a_at, void * ap_data)
{
    if (!ap_array) return PR_FALSE;

    while (a_at >= ap_array->capacity) {
        char * lp_data = ap_array->data;
        lp_data = realloc(lp_data, ap_array->capacity * 2 * ap_array->stride);
        if (lp_data) {
            pr_u32_t l_i;
            ap_array->data      = lp_data;
            ap_array->capacity  *= 2;
            for (l_i=ap_array->size ; l_i < ap_array->capacity ; l_i++) {
                ap_array->initializer(&ap_array->data[l_i * ap_array->stride], ap_array->stride);
            }
        } else {
            return PR_FALSE;
        }
    }
        
    if (ap_data) {
        memcpy(&ap_array->data[a_at * ap_array->stride], ap_data, ap_array->stride);
    } else {
        ap_array->initializer(&ap_array->data[a_at * ap_array->stride], ap_array->stride);
    }

    ap_array->size = a_at + 1;

    return PR_TRUE;
}

Pr_BitSet * Pr_NewBitSet()
{
    return Pr_NewArray(sizeof(pr_u32_t), NULL);
}

pr_bool_t Pr_SetBitSetAt(Pr_BitSet * ap_bitset, pr_u32_t a_at, char bit)
{
    if (!ap_bitset) return PR_FALSE;

    while (a_at >= (ap_bitset->capacity * 32)) {
        void * lp_data = ap_bitset->data;
        lp_data = realloc(lp_data, ap_bitset->capacity * 2 * ap_bitset->stride);
        if (lp_data) {
            ap_bitset->data      = lp_data;
            ap_bitset->capacity  *= 2;
            for ( ; ap_bitset->size < a_at ; ap_bitset->size++) {
                ap_bitset->initializer(&ap_bitset->data[ap_bitset->size], ap_bitset->stride);
            }

            ap_bitset->size = a_at + 1;

        } else {
            return PR_FALSE;
        }
    }

    if (bit) {
        ap_bitset->data[a_at / 32] |= (1 << a_at % 32);
    } else {
        ap_bitset->data[a_at / 32] &= ~(1 << a_at % 32);
    }

    return PR_TRUE;
}
