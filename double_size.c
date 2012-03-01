#include <stdio.h>

main()
{
    int i= 0;
    int ibak = 0;
    long l;
    double d;
    long double ld;
    float f;
    char c;
    printf("Size of i is %d\n", sizeof(i));
    printf("Size of l is %d\n", sizeof(l));
    printf("Size of d is %d\n", sizeof(d));
    printf("Size of ld is %d\n", sizeof(ld));
    printf("Size of f is %d\n", sizeof(f));
    printf("Size of c is %d\n", sizeof(c));


    
    ibak = 1;
    while (ibak > 0)
    {
        ibak++;
        i++;
    }
    printf("The highest value of i is %ld\n", i);
 
    ibak = 1;
    i = 1;
    do
    {
        i++;
        ibak++;
    } while (ibak > 0);
    printf("The highest value of i is %d\n", i);
       
    ibak = 0;
    ibak = ~ibak;
    printf("The highest value of i is %d\n", ibak);
}
