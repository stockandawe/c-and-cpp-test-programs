#include <stdio.h>

int binary_num = 72;

int tsString_GetHexCharVal(const char hexChar, unsigned char* pVal)
{
    if (!pVal)
    { 
        return 0;
    }
    
    if ((hexChar >= '0') && (hexChar <= '9'))
    {
        *pVal = hexChar - '0';
    }
    else if ((hexChar >= 'a') && (hexChar <= 'f'))
    {
        *pVal = hexChar - 'a' + 10;
    }
    else if ((hexChar >= 'A') && (hexChar <= 'F'))
    {
        *pVal = hexChar - 'A' + 10;
    }
    else
    {
        return 0;
    }

    return 1;
}

int main(void)
{
    char hex_str[2];
    unsigned char a;
    unsigned char b;
    
    printf("Converting %d to hex\n", binary_num);
    sprintf(hex_str, "%x", binary_num);
    printf("The result is %c%c\n", hex_str[0], hex_str[1]);
    tsString_GetHexCharVal(hex_str[0], &a);
    tsString_GetHexCharVal(hex_str[1], &b);
    printf("a = %x, b = %x\n", a, b);

    if (binary_num > 15)
    {
    if ((b & 8) > 0)
    {
        printf("This is a switch\n");
    }
    }
    else
    {
    if ((a & 8) > 0)
    {
        printf("This is a switch\n");
    }
    }
    return 1;
}
    
