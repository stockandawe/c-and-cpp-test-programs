#include <stdio.h>

void my_strncat (char *DST, const char *SRC, int MAX_LENGTH)
{
    int count_dst = 0;
    int count_src = 0;

    while(DST[count_dst] != '\0')
    {
        count_dst++;
    }

    while (SRC[count_src] != '\0')
    {
        if (count_dst + count_src > MAX_LENGTH)
        {
            DST[count_dst] = '\0';
            return;
        }

        DST[count_dst++] = SRC[count_src++];
    }
    return;
}

int main(void)
{
    char dst[6] = {"Hello"};
    char src[] = {"World"};
    int max_len = 8;

    printf("Dst is now %s\n", dst);
    my_strncat(dst, src, max_len);
    printf("Dst is now %s\n", dst);

    return 1;
}
