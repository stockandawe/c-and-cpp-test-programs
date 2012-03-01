#include <stdio.h>
#include <string.h>

/*
 * given a number, print
 * 0 3......... 
 * 1 4........
 * 2 5..........
 */
int main(int argc, char **argv)
{
    int num;
    int i;
    char tmp[256];
    char str1[256];

    if (argc < 2)
    {
        printf("Error: Usage: \n");
        exit(-1);
    }
    num = atoi(argv[1]);

    for (i = 0; i < num; i=i+3)
    {
        sprintf(tmp, " %3d", i);
        strcat(str1, tmp);
    }
    printf("%s\n", str1);
    str1[0] = '\0';
    for (i = 1; i < num; i=i+3)
    {
        sprintf(tmp, " %3d", i);
        strcat(str1, tmp);
    }
    printf("%s\n", str1);
    str1[0] = '\0';
    for (i = 2; i < num; i=i+3)
    {
        sprintf(tmp, " %3d", i);
        strcat(str1, tmp);
    }
    printf("%s\n", str1);

    return 1;
}
