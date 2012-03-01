#include <stdio.h>

int count_ones(unsigned int n)
{
    int count = 0;
    int size = sizeof(n) * 8;
    int i;
    unsigned int test;

    test = n;
    for (i = 0; i < size; i++)
    {
        if (test & 1)
        {
            count++;
        }
        n = test = n >> 1;
    }
    return count;
}

int count_ones_v1(unsigned int n)
{
    int count = 0;
    while(n)
    {
        if (n & 0x1u)
        {
            count++;
        }
        n >>= 1;
    }
    return count;
#if 0
    int count=0;    
      while (n)
      {
         count += n & 0x1u ;
         n >>= 1 ;
      }
      return count ;
#endif
}

int based_on_num_of_ones(unsigned int n)
{
    int count = 0;
    while (n)
    {
        count++;
        n &= (n-1);
    }
    return(count);
}

int based_on_num_of_zeros(unsigned int n)
{
    int count;
    count = sizeof(unsigned int) * 8;

    n = ~n;
    while (n)
    {
        count--;
        n &= (n-1);
    }
    return count;
}

void show_binary(unsigned int n)
{
    while(n)
    {
        printf("%d", n&1);
        n >>= 1;
    }
    return;
}
int main(int argc, char ** argv)
{
    unsigned int input;

    if (argc < 2)
    {
        printf("Usage: countones <unsigned int>\n");
        exit(-1);
    }

    input = atoi(argv[1]);
    printf("Input is %d\n", input);

    printf("Number of ones is %d\n", count_ones(input));        
    printf("Number of ones is %d\n", count_ones_v1(input));
    show_binary(input);
    printf("\n");
    printf("Number of ones is %d\n", based_on_num_of_ones(input));
    printf("Number of ones is %d\n", based_on_num_of_zeros(input));
}
