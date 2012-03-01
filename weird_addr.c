#include <stdio.h>

#define ADDR_STR "1.1.204.200"

int main()
{
    char decode_str[32];
    int   a, b, c, d;

    printf("starting program....\n");

    printf("decode addr_str %s\n", ADDR_STR);

    sscanf(ADDR_STR, "%d.%d.%d.%d", &a, &b, &c, &d);

    sprintf(decode_str, "%x.%x.%x.%x\n", 
        (a), 
        (b), 
        (c), 
        (d));

    printf("decoded addr_str %s\n", decode_str);

    return 1;
}

