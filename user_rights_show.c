#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>


/* Global declarations */
char UserPermissions[512][20];

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
    
    while (n < 3) 
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

void
initializeUserLevel(void)
{
    int i;

    /* Initialize whole table with Error string */
    for (i = 0; i < 512; i++)
    {
        strcpy(UserPermissions[i], "***Error***");
    }

    strcpy(UserPermissions[1], "General RO-User");
    strcpy(UserPermissions[2], "General RW-User");
    strcpy(UserPermissions[4], "IB RO-User");
    strcpy(UserPermissions[8], "IB RW-User");
    strcpy(UserPermissions[16], "ETH RO-User");
    strcpy(UserPermissions[20], "IB | ETH RO-User");
    strcpy(UserPermissions[32], "ETH RW-User");
    strcpy(UserPermissions[40], "ETH | IB RW-User");
    strcpy(UserPermissions[64], "FC RO-User");
    strcpy(UserPermissions[128], "FC RW-User");
    strcpy(UserPermissions[256], "Unrestricted RW-User");
}

char *showUserLevel(char *inputStr)
{
    int Index;
    char formatInputStr[6];
    char hexString[3];

    if (strstr(inputStr, "0x") == NULL)
    {
        return("***Error***");
    }
    if (strlen(inputStr) < 6)
    {
        sprintf(formatInputStr, "0x0%s", inputStr+2);
    }
    else
    {
        sprintf(formatInputStr, "%s", inputStr);
    }
    strncpy(hexString, formatInputStr+2, strlen((formatInputStr) -3));

    Index = axtoi(hexString);
    return(UserPermissions[Index]);

#if 0

        if (strcmp(inputStr, "0x01,") == 0)
        {
            return(UserPermissions[1]);
        }
        else if (strcmp(inputStr, "0x02,") == 0)
        {
            return("General RW-User");
        }
        else if (strcmp(inputStr, "0x100,") == 0)
        {
            return("OBER-User");
        }

        return("***Error***");
#endif
}

int
main(void)
{
    FILE *fp;
    char Input[512];
    char LineInput[512];
    char nodeName[100];
    char tmp[100];
    int  tmpInt;
    char userLevel[20];

    fp = fopen("rcc_cli.c", "r");
    if (fp == NULL)
    {
        printf("Error opening file\n");
        exit(0);
    }
    initializeUserLevel();
    
    while(fgets(Input, sizeof(Input), fp) != NULL)
    {
        if (strstr(Input, "static cmdNode mClearEthernetChildren"))
        {
//            fgets(LineInput, sizeof(LineInput), fp);
//            while (fgets(LineInput, sizeof(LineInput), fp) != NULL)
//            {
//                printf("%s\n", LineInput);
//            }
//        }
                      
#if 1
            printf("%-20s %-30s\n", "Command", "Minimum User Permisssion Level");
            printf("--------------------------------------------------\n");
            fgets(LineInput, sizeof(LineInput), fp);
            while (fgets(LineInput, sizeof(LineInput), fp) != NULL)
            {
                sscanf(LineInput, 
                    "    { %s %s %s %s %s %s %s %s %s %s %s %s },", 
                    nodeName, tmp, tmp, tmp, tmp, userLevel, tmp, tmp, tmp, 
                    tmp, tmp, tmp);

                printf("%-20s %-30s\n", 
                    nodeName, showUserLevel(userLevel));
            }
        }
#endif
    }
    return;
}
