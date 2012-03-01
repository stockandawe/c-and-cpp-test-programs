#include <stdio.h>

int divide(int num, int denom) {
  if (num < denom) return 0;
  return (1 + divide(num - denom, denom));
}

/*
 * Follow this process until you run out of bits in the numerator:

1)  Chop the leftmost bit of the numerator off and stick it to the right of a.
2)  Extend b with a 0 (on the right).
3)  If a is greater than or equal to the denominator, subtract the denominator from it and increment b.

After you're done with that loop, b holds your answer.
*/
int divide_2(int num, int denom)
{
    int a, b;
    int i = sizeof(int) * 8 - 1;

    while(i >= 0)
    {
        a = (a << 1) + ((num & (1 << i)) >> i);
        b = b << 1;
        if (a >= denom)
        {
            a -= denom;
            b++;
        }
        i--;
    }
    return b;
}

int main()
{
    int num = 162, den = 8;
    printf("Dividing %d by %d = %d\n", 
        num, 
        den, 
        divide_2(num, den));

    return 1;
}
