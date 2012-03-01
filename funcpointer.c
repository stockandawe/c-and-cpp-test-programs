#include <stdio.h>
#if 0
typedef int (*functiontype)(int);

int foo2(int bar)
{
    return(bar * 2);
}
int foo3(int bar)
{
    return(bar * 3);
}
int foo4(int bar)
{
    return(bar * 4);
}

int main()
{
    functiontype function;

    function = foo2;
    printf("\n5 times 2 is %d\n", (function)(5));

    function = foo3;
    printf("\n5 times 3 is %d\n", (function)(5));

    function = foo4;
    printf("\n5 times 4 is %d\n", (function)(5));

}
#endif

typedef void (*functiontype)(void);

void A(void)
{
    printf("In function A\n");
}

void B(void)
{
    printf("In function B\n");
}

int main()
{
    functiontype fun_array[3] = {A, B, C};
//    fun_array[0] = A;
//    fun_array[1] = B;

    (fun_array[0])();
    (fun_array[1])();
    (fun_array[2])();

    return 1;
}
