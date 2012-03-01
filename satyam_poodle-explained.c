#include <stdio.h>
#include <string.h>

struct ID {
    int one;
};

typedef struct ID * ID;

int equal(ID a, ID b)
{
    return (a->one == b->one);
}

/*
 * Since we have the opaque data structure, the logic is to traverse the 
 * array and use equal() for comparision. 
 * However, optimize (explained below) as much as possible.
 */
int poodle(ID * ids, int n)
{
    int   outer, inner;
    int   match = 0;
    int   counts[n]; // some platforms allow this. absolutely portable use malloc
        
    memset(counts, 0xff, sizeof(counts));

#define OPTIMIZE2  // saves outer loops if only n/2 elements are remaining 

#ifdef OPTIMIZE2
    for (outer = 0; outer < n - (n/2); outer++) 
#else
    for (outer = 0; outer < n - 1; outer++)
#endif
    {
        // OPTIMIZE1: this has matched something earlier, just skip it
        if (counts[outer] != 0xffffffff)
        {
            continue;
        }

        // initialize
        match = counts[outer] = 1;

        for (inner = outer + 1; inner < n; inner++)
        {
            if (equal(ids[outer], ids[inner]))
            {
                counts[outer]++;

#define OPTIMIZE1  // saves a bunch of outer comparisons

#ifdef OPTIMIZE1
                counts[inner] = 1;
#endif
                if (counts[outer] > match)
                    match = counts[outer];
                
                if (match > n/2)
                {
                    return 1;
                }
            }

#define OPTIMIZE3 // avoids looking at rest of the elements -no chance of success

#ifdef OPTIMIZE3
            /* 
             * the inner iteration does not have enough elements remaining
             * that it could possibly return true so advance to next in the
             * outer loop
             */
            if ((n - (inner + 1)) < (n/2 - match + 1))
                break;
#endif
        }
    }
    
    return 0;
}

int main()
{
    int i;

#if 1
    #define NUM_ELEMENTS    11

    struct ID  array1[NUM_ELEMENTS]      = { 1, 1, 2, 2, 2, 3, 3, 3, 3, 3, 3 };
#else 
    #define NUM_ELEMENTS    18

    struct ID  array1[NUM_ELEMENTS]      = { 1, 1, 2, 2, 2, 3, 3, 3, 3, 4, 5, 6, 7, 8, 8, 9, 9, 9 };
#endif

    ID         array_refs[NUM_ELEMENTS];

    for (i = 0; i < NUM_ELEMENTS; i++)
    {
        array_refs[i] = &array1[i];
    }

    if (poodle(array_refs, NUM_ELEMENTS))
        printf("Enough matches\n");
    else 
        printf("Not enough matches\n");

    return;
}
