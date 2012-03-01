// Question #2:
/* Write an implementation for the following heap methods
 * using a buddy allocator to manage the specified memory
 */

#include <unistd.h>
#include <stdio.h>

#define CHUNKSIZE 16 * 1024 * 1024 

#define L_BLOCKSIZE    1 * 1024
#define U_BLOCKSIZE   16 * 1024 * 1024

typedef struct heap {
    int            size : 31;
    int            used :  1;
    struct heap *  next;
    struct heap *  prev;
} heap;


/* Returns an initialized heap state pointer. The memory for the
 * heap structure should be allocated from the specified memory 
 */
heap *
heap_init(void *mem, long size)
{
    heap * h = (heap *) mem;

    h->size = size;
    h->used = 0;
    h->prev = NULL;
    h->next = NULL;
}

/* Returns a pointer a memory segment that can accomodate as least
 * size number of bytes. On error, returns NULL
 */
void *
heap_alloc(heap * h, long size) 
{
    int     computed = L_BLOCKSIZE;
    heap *  bigger   = NULL;
    heap *  iterator = h;

    size += sizeof(heap);

    if (size > U_BLOCKSIZE)
        return NULL;

    while (size > computed)
    {
        computed <<= 1;
    }

    printf ("Size %08x Need %08x\n", size, computed);

    // Traverse to find if we have one 
    while (iterator)
    {
        if (!iterator->used)
        {
            if (iterator->size == computed)
            {
                printf ("Found %p\n", iterator);
                break;
            }

            if (iterator->size > computed)
            {
                bigger = iterator;
            }
        }

        iterator = iterator->next;
    }
    
    if (!iterator && bigger)
    {
        // Split the big cell 
        iterator = bigger;
        while (iterator->size > computed)
        {
            heap * next = NULL;
            
            iterator->size = iterator->size >> 1;
            
            next = (heap *) ((char *) iterator + iterator->size);

            // printf ("Heap %p  Next %p Size %08x Computed %08x\n", iterator, next, iterator->size, computed);
            
            next->size = iterator->size;
            next->prev = iterator;
            next->next = iterator->next;

            iterator->next = next;
        }
        printf ("Split to find %p\n", iterator);
    }
    
    if (iterator)
    {
        iterator->used = 1;
        return (void *) ((char *) iterator + sizeof(heap));
    }

    return NULL;
}

void 
heap_print(heap *h)
{
    while (h)
    {
        printf("LOCATION:%p SIZE:%08x %s\n", h, h->size, h->used ? "Used" : "Free");
        h = h->next;
    }
}

/* Return memory back to the heap
 */
void
heap_free(heap *h, void *ptr)
{
    heap * block = NULL;
    heap *neighbor_right;
    heap *neighbor_left;

    if ((heap *)ptr < h)
    {
        perror("heap_free");
        return; 
    }

    block = (heap *) ((char *) ptr - sizeof(heap));

    block->used = 0;

    /* Compact the memory */
    neighbor_right = block->next;
    while (neighbor_right)
    {
        if (neighbor_right->used)
        {
            break;
        }
        block->next = neighbor_right->next;
        block->size += neighbor_right->size;
        neighbor_right = neighbor_right->next;
    }

    neighbor_left = (heap *) block->prev;
    while (neighbor_left)
    {
        if (neighbor_left->used)
        {
            break;
        }

        neighbor_left->next = block->next;
        neighbor_left->size += block->size;
        neighbor_left = neighbor_left->prev;
    }
}


int main()
{
    void *mem;
    heap *h;
    mem = sbrk(CHUNKSIZE);
    void *ptr1; void *ptr2; void *ptr3;

    if (!mem) { 
        perror("sbrk");
        return 1;
    }

    h = heap_init(mem, CHUNKSIZE);
 
    printf("initial state\n");
    heap_print(h);
   
    ptr1 = heap_alloc(h, 1 * 1024 * 1024);
    ptr2 = heap_alloc(h, 3 * 1024 * 1024);
    ptr3 = heap_alloc(h, 3 * 1024 * 1024);
    
    heap_print(h);

    printf("free at %p\n", (char *)ptr1 - sizeof(heap));
    heap_free(h, ptr1);
    printf("free at %p\n", (char *)ptr2 - sizeof(heap));
    heap_free(h, ptr2);

    printf("after free...\n");
    heap_print(h);
    ptr1 = heap_alloc(h, 3 * 1024 * 1024);
    ptr2 = heap_alloc(h, 3 * 1024 * 1024);
    heap_print(h);
    printf("free at %p\n", (char *)ptr1 - sizeof(heap));
    heap_free(h, ptr1);
    printf("free at %p\n", (char *)ptr2 - sizeof(heap));
    heap_free(h, ptr2);
    printf("after free...\n");
    heap_print(h);

    printf("free at %p\n", (char *)ptr3 - sizeof(heap));
    heap_free(h, ptr3);
    printf("after free...\n");
    heap_print(h);

    return 0;
}
