#include <stdio.h>

struct ID {
    int one;
};

typedef struct ID * ID;

int equal(ID a, ID b)
{
    return (a->one == b->one);
}

int poodle(ID * ids, int n)
{
    int   outer, inner;
    int   max = 0;
    int   counts[n];         // some platforms allow this. to be absolutely portable use malloc
        
    memset(counts, 0xff, sizeof(counts));

    for (outer = 0; outer < n - (n/2); outer++) 
    {
        // optimization: this has matched something earlier, just skip it
        if (counts[outer] != 0xffffffff)
            continue;
        
        // initialize
        max = counts[outer] = 1;

        for (inner = outer + 1; inner < n; inner++)
        {
            if (equal(ids[outer], ids[inner]))
            {
                counts[outer]++;

                counts[inner] = 1;   // skip this in the outer loop 

                if (counts[outer] > max)
                    max = counts[outer];
                
                if (max > n/2)
                    return 1;

                // the inner iteration does not have enough elements remaining
                // that it could possibly return true so advance to next in the
                // outer loop 
                if ((n - (inner + 1)) < (n/2 - max + 1)) 
                    break;
            }
        }
    }
    
    return 0;
}

int main()
{
    int i;

#if 0
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
