#include <stdio.h>

#if 0
int multiply (int a, int b)
{
    int answer = 0;
    int negative = 0;

    if (a < 0)
    {
        /* 2's compliment to convert negative to positive */
        a = ~a + 1;
        negative = 1;
    }
    if (b < 0)
    {
        /* 2's compliment to convert negative to positive */
        b = ~b + 1;
        negative ^= 1;
    }

    if (b > a)
    {
        /* 
         * If b is greater than a, 
         * swap the two numbers 
         */
        b = b + a;
        a = b - a;
        b = b - a;
    }

    /* 
     * the loop iterates b number of times.
     * Hence making sure that b is always smaller 
     * than a (above) is an optimization
     */
    while(b != 0)
    {
        answer = answer + a;
        b--;
    }

    if (negative)
    {
        /* 2's compliment to convert negative to positive */
        answer = ~answer + 1;
    }
    return(answer);
}
#endif



int main()
{
    printf("multiply answer is %d\n", multiply(-50, 0));
    return 1;
}
