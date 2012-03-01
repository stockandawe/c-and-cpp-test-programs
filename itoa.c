#include <stdio.h>
#include <string.h>

int value = 1234;

/* 
 * reverse:  reverse string s in place 
 */
void reverse(char s[])
{
    int c, i, j;
    
    for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

/*
 * Simple implementation of itoa.
 * Still Needs: 
 * 1. take care of sign (for negative inputs)
 */
char *my_itoa(int value, char *str)
{
    int i = 0;
    while (value)
    {
//        printf("str[%d] = %d\n", i++, value % 10);
        str[i++] = (value % 10) + '0';
        value = value / 10;
    }
    str[i] = '\0';
    return str;
}

int main()
{
    char str[8];
    printf("Reversed string is %s\n", my_itoa(value, str));
    reverse(str);
    printf("Desired string is %s\n", str);

    return 1;
}
    
