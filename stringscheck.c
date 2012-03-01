#include <stdio.h>
#include <string.h>

void printStr(char *str_ptr)
{
    int str_len;

    str_len = strlen(str_ptr);
    printf("The string is %s of length %d\n", str_ptr, str_len);

    return;
}

int wordsInString(char *str_ptr)
{
    int num_of_words = 0;
    int i = 0;

    while (str_ptr[i] != '\0')
    {
        if (str_ptr[i] == ' ')
        {
            num_of_words++;
        }
        i++;
    }

    if (str_ptr[0] != '\0')
    {
        num_of_words++;
    }

    return(num_of_words);
}

int main()
{
    char str[] = {"A"};
    printf("Starting string manipulation program\n");

    printStr(str);

    printf("String has %d number of words\n", wordsInString(str));
}
    
