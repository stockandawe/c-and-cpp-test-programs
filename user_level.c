#include <stdio.h>

int main()
{
    int x = 0xf0;
    int y = 0x00;
//    int a = 0x20;
    int z;

    printf("Size of int is %d\n", sizeof(int));

    z = x | y; 
    printf("value of x is 0x%0x\n", z);
    printf("which is = %d\n", z);

    return 1;

}
