#include <stdio.h>
#include <string.h>


/*
 * axtoi():
 * Function to convert hexString (2 bytes) to it's corresponding 
 * integer value.
 * Example: 0a = 10, 0b = 11, 0c = 12, 0d = 13.....1f = 31
 */
int axtoi(char *hexStg) {
    int n = 0;         // position in string
    int m = 0;         // position in digit[] to shift
    int count;         // loop index
    int intValue = 0;  // integer value of hex string
    int digit[5];      // hold values to convert
    
    while (n < 2) 
    {
        if (hexStg[n]=='\0')
            break;
        if (hexStg[n] > 0x29 && hexStg[n] < 0x40 ) //if 0 to 9
            digit[n] = hexStg[n] & 0x0f;            //convert to int
        else if (hexStg[n] >='a' && hexStg[n] <= 'f') //if a to f
            digit[n] = (hexStg[n] & 0x0f) + 9;      //convert to int
        else if (hexStg[n] >='A' && hexStg[n] <= 'F') //if A to F
            digit[n] = (hexStg[n] & 0x0f) + 9;      //convert to int
        else break;
     
        n++;
    }
    count = n;
    m = n - 1;
    n = 0;
    while(n < count) 
    {
        // digit[n] is value of hex digit at position n
        // (m << 2) is the number of positions to shift
        // OR the bits into return value
        intValue = intValue | (digit[n] << (m << 2));
        m--;   // adjust the position to set
        n++;   // next digit to process
    }
    
    return (intValue);
}

int main()
{
    char hexStr[2];
    int  dec;
    
    printf("Starting string hex to decimal program\n");

    strcpy(hexStr, "188");
    dec = axtoi(hexStr);

    printf("decimal value of %s is %d\n", hexStr, dec);
    return 1;
}

    
