#include <stdio.h>

char src_str[] = {"Hello How are you today!"};

void swap(char *x, char *y)
{
    char tmp;
    tmp = *x;
    *x = *y;
    *y = tmp;
}

int main()
{
    int len=0;
    char tmp;
    char *head, *tail;
    char *i;
    char *orig_str = src_str;

    /*
     * Step 1: Reverse the entire string
     */
    len = strlen(src_str);
    head = src_str;
    tail = head + len - 1;

    while (head < tail)
    {
        swap(head++, tail--);
    }
    src_str[len] = '\0';
    printf("New string is %s\n", orig_str);

    /*
     * Step 2: Reverse each word in the reversed string
     */
    while (*orig_str)
    {
        i = orig_str;
        head = orig_str;
        while (*i != '\0' && (*i != ' '))
        {
            i++;
        }
        tail = i-1;
        while (head < tail)
        {
            swap(head++, tail--);
        } 
        orig_str = i + 1;
    }
    printf("New string is %s\n", src_str);
}
