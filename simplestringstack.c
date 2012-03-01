#include <stdio.h>
#include <string.h>

char* a_static(char *string)
{
    static char a[16];
    strcpy(a, string);
    return a;
}

char *a(char *str)
{
    char a[16];
    strcpy(a, str);
    return a;
}

int main()
{
    printf("\t%s \t%s", a("hello"), a("world"));
    printf("\t%s \t%s", a_static("hello"), a_static("world"));

    return 1;
}
