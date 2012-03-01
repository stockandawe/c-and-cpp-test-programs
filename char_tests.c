#include <stdio.h>

int main(void)
{
    const char *c = {"Hellohello"};
    char * const c2 = {"Worlddsfafdasfdafds"};
    int i = 0;
    int size;

    size = sizeof(c);
    printf("\nsizeof c is %d\n", size);
    while(*(c) != '\0')
    {
        printf("%c", *(c++));
        i++;
    }
    i = 0;
    printf("\nsizeof c2 is %d\n", sizeof(c2));
    while(i <= sizeof(c2))
    {
        printf("%c", c2[i]);
        i++;
    }

    /* 
     * This will not compile
     * warning: assignment of read-only location
     */
#if 0
    while(i <= sizeof(c))
    {
        *(c++) = 'c';
        i++;
    }
#endif
}
