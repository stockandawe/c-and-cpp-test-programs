#include <stdio.h>

int main()
{
    char buf[100];
    int i;
    
    printf("Starting....\n");

    buf[0] = '\0';

    for(i=0; i<100; i++)
    {
        printf("%c", buf[i]);
    }
    printf("Size of buf is %d\n", sizeof(buf));

    memset(buf, 0, 100);
 
    for(i=0; i<100; i++)
    {
        printf("%c", buf[i]);
    }
    printf("Size of buf is %d\n", sizeof(buf));
   
    
    return;
}
    
