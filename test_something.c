#include <stdio.h>

int* fun1(void)
{
    int *i;
    i = (int *)malloc(sizeof(int));
    *i = 100;
    return(i);
}
int fun2(void)
{
    int i;
    i = 100;
    return(i);
}

int main()
{
    int *i;
    int j;
    i = fun1();
    j = fun2();
    int x;
//    printf("fun1 returns %d\n", fun1());
    printf("i is %d\n", *i);
    printf("j is %d\n", j);
}
