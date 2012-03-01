#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

int main()
{
    FILE    *fp = NULL; 
    int     counter = 0;
    
    printf("starting filewrite program\n");

    if ((fp = fopen("/data/home/rutul/test_progs/tmp1", "a+")) == NULL)
    {
        printf("ERROR");
        return 0;
    }

    while(1)
    {
        fprintf(fp, "a");
    }

    fclose(fp);

    return 1;
}

