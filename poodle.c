#include <stdio.h>
#include <string.h>

struct ID {
    int one;
};

typedef struct ID* ID;

int equal(struct ID a, struct ID b)
{
    if (a.one == b.one)
        return 1;
    return 0;
}

int poodle(struct ID *ids, int n)
{
    struct ID processed_ids[n];
    int i; int j; int is_equal; int total_unique = 0;
    int processed;

    memset(processed_ids, 0, sizeof(struct ID)*n);
    for (i = 0; i < n; i++)
    {
        processed = 0;
        for (j = 0; j <= total_unique; j++)
        {
            if (equal(ids[i], processed_ids[j]))
            {
                processed = 1;
                break;
            }
        }
        if (processed)
            continue;

        memcpy(processed_ids+j, ids+i, sizeof(struct ID));
        total_unique++;

        is_equal = 1;
        for (j = i+1; j < n; j++)
        {
            if (equal(ids[i], ids[j]))
                is_equal++;
            if (is_equal >= n/2)
                return 1;
        }
    }

    return 0;
}

int main()
{
    int n = 9;
    struct ID array1[n];
    int i;
    array1[0].one = 1; array1[1].one = 1;
    array1[2].one = 2; array1[3].one = 2, array1[4].one = 2;
    array1[5].one = 3; array1[6].one = 3, array1[7].one = 3, array1[8].one = 3;
#if 0
    printf("orig array is:\n");
    for (i = 0; i < n; i++)
    {
        array1[i].one = 100;
        printf("%d\n", array1[i].one);
    }
#endif

    if (poodle(array1, n))
        printf("IDs are identical\n");

    return;
}
