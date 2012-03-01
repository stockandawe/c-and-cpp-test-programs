#include <stdio.h>

int main()
{
    int value = 1 *1024;
    printf("left shit is %d\n", value <<= 1);
    printf("right shit is %d\n", value >>= 1);

    return 1;
}

        
