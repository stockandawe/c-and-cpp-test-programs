typedef struct heap heap;

struct heap {
    /* free areas in heap */
    struct free *free;
    /* lock for mult-threading */
    pthread_mutex_t lock;
};
    
struct free {
        long size;
        struct free *next, *prev;
};

#define __heap_lock(heap)   __pthread_mutex_lock (&(heap)->lock)
#define __heap_unlock(heap) __pthread_mutex_unlock (&(heap)->lock)

/* Returns an initialized heap state pointer. The memory for the
 *  * heap structure should be allocated from the specified memory
 *   */
heap *
heap_init(void *mem, long size)
{
    heap *heap = NULL;
    free *free_area = NULL;

    heap = (heap *)malloc(sizeof(heap));
    if (heap == NULL)
        return NULL;
    
    free_area = heap->free = heap;
    free_area->size = size;
    free_area->next = free_area->prev = NULL;
    
    return heap;
}


/* Returns a pointer a memory segment that can accomodate as least
 * size number of bytes. On error, returns NULL
 */
 void *
 heap_alloc(heap *h, long size)
{
    struct free *fa;
    void *mem = NULL;

    /* look for a free area.  */
    for (fa = h->free; fa; fa = fa->next)
    {
        if (fa->size >= size)
        {
            /* enough free area available  */
            mem = (void *)fa;
            fa->size = fa->size - size;
            break;
        }
    }
        
    return mem;
}


-----
/* Return memory back to the heap
 */
 void
heap_free(heap *h, void *ptr)
{
    struct free *fa, *prev_fa;
    void *end = (char *)mem + size;

    if (fa && HEAP_FREE_AREA_START (fa) <= end)
        /* The free-area FA is adjacent to the new block, merge them.  */
    {
        size_t fa_size = fa->size + size;
        
        if (HEAP_FREE_AREA_START (fa) == end)
            /* FA is just after the new block, grow down to encompass it. */
        {
            /* See if FA can now be merged with its predecessor. */
            if (prev_fa && mem == HEAP_FREE_AREA_END (prev_fa))
                /* Yup; merge PREV_FA's info into FA.  */
            {
                fa_size += prev_fa->size;
                __heap_link_free_area_after (heap, fa, prev_fa->prev);
        }
        else
            /* FA is just before the new block, expand to encompass it. */
        {
            struct heap_free_area *next_fa = fa->next;
            
            /* See if FA can now be merged with its successor. */
            if (next_fa && end == HEAP_FREE_AREA_START (next_fa))
                /* Yup; merge FA's info into NEXT_FA.  */
            {
                fa_size += next_fa->size;
                __heap_link_free_area_after (heap, next_fa, prev_fa);
                fa = next_fa;
            }
            else
                /* FA can't be merged; move the descriptor for it to the tail-end
                 *         of the memory block.  */
            {
                /* The new descriptor is at the end of the extended block,
                 *         SIZE bytes later than the old descriptor.  */
                fa = (struct heap_free_area *)((char *)fa + size);
                /* Update links with the neighbors in the list.  */ 
                __heap_link_free_area (heap, fa, prev_fa, next_fa);
            }
        }
        fa->size = fa_size;
    }
    else
        /* Make the new block into a separate free-list entry.  */
        fa = __heap_add_free_area (heap, mem, size, prev_fa, fa);

    
    return fa;
}



 int main()
 {
     void *mem;
     heap *h;

     mem = sbrk(CHUNKSIZE);
     if (!mem) {
         perror("sbrk");
         return 1;
     }
     
     h = heap_init(mem, CHUNKSIZE);
     
     /* calls to heap_alloc/heap_free */
     
     return 0;
 }

struct heap {
    block_list *free_blocks;
};

struct block_list {
    long size;
    block_list *next;
}

heap *
heap_init(mem, size)
{
    /* create 1 free_blocks with size */

    /* return mem */
}

void * heap_alloc(heap *h, long size)
{
    /* loop through heap->free_blocks looking for (free_blocks->size > MIN) 
     * and (size < free_blocks->size)
     */

    /*
     * when found, divide up that free_block creating */ 
}

void heap_free(heap *h, ptr)
{
    blocks = h->free_blocks;
    while (blocks)
    {
        if (blocks == ptr)
            break;

        blocks = blocks->next;
    }

    /* combine neighbors */
}
