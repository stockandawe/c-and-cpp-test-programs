#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/*======================*/
char* Asc2Hex(char *str) {
 int i;
 char* answer;
 answer = (char*) malloc((2*strlen(str)+1) * sizeof(char));

 for(i=0; str[i]!=0; i++) {
   sprintf(answer+(i*2), "%X", str[i]);
 }

 return answer;
}
/*======================*/


/*=================*/
int toDigit(char c) {
 if (c>=48 && c<=57) {
   return (int) c-48;
 }
 else if (c>=65 && c<=70) {
   return (int) c-55;
 }
 return(c);
}
/*=================*/


/*======================*/
char* Hex2Asc(char *str) {
 int i;
 char* answer;
 char tmp1[]={0,0,0}, tmp2[]={0,0,0};
 answer = (char*) malloc((strlen(str)/2+1) * sizeof(char));
   
 for(i=0; str[i]!=0; i+=2) {
   sprintf(tmp1, "%d", toDigit(str[i]));
   sprintf(tmp2, "%d", toDigit(str[i+1]));

   answer[i/2] = atoi(tmp1) * 16 + atoi(tmp2);
 }

 return answer;
}
/*======================*/

 
/*========*/   
int main(int argc, char *argv[]) {
 
#if 0 
 char strhex[100] = {"088300320201002258595A5F436F7270537570657220547572626F204469736B3230333435383933343501020080"};
 char strascii[100] = {"0883003102010022IBM CorpNot a disk      35354345345345345345345345"};
 char strType[3];
 char vendorId[18];
 char diskDescr[34];
 char serialNum[22];
 char returnBuf[100];
 char str[100];

 if (argc < 2)
 {
     printf("Usage : ascii <strhex | strascii>\n");
     exit(0);
 }
 
 if (strcmp(argv[1], "strhex") == 0)
 {
     memcpy(str, strhex, 100);
 }
 else if (strcmp(argv[1], "strascii") == 0)
 {
     memcpy(str, strascii, 100);
 }
 else
 {
     printf("Usage : ascii <strhex | strascii>\n");
     exit(0);
 }

 strType[0] = '\0';
 vendorId[0] = '\0';
 diskDescr[0] = '\0';
 serialNum[0] = '\0';
 returnBuf[0] = '\0';
 memcpy(strType+0, str+6, 2);
 strType[2] = '\0';

 if (strcmp(strType, "31") == 0)
 {
     memcpy(vendorId, str+16, 8);
     vendorId[8] = '\0';
     memcpy(diskDescr, str+24, 16);
     diskDescr[16] = '\0';
     strncpy(serialNum, str+40, 10);
     serialNum[10] ='\0';
 }
 else 
 {
     memcpy(vendorId+0, str+16, 18);
     vendorId[16] = '\0';
     memcpy(diskDescr+0, str+32, 34);
     diskDescr[32] = '\0';
     strncpy(serialNum, str+64, 22);
     serialNum[20] ='\0';
 }
 
 printf("Type is %s\n", Hex2Asc(strType));
 if (strcmp(strType, "31") == 0)
 {
     sprintf(returnBuf, "%s %s %s", vendorId, diskDescr, serialNum);
 }
 else
 {
     sprintf(returnBuf, "%s %s %s",  Hex2Asc(vendorId), Hex2Asc(diskDescr), Hex2Asc(serialNum));
 }
// printf("Vendor Id is %s\n", Hex2Asc(vendorId)); 
// printf("Disk Descr is %s\n", Hex2Asc(diskDescr));
// printf("Serial num is %s\n", Hex2Asc(serialNum));
    
#endif

    char strType;
    char str[64];
     char returnBuf[100];
   char localBuf[64];
   char hexBuf[56];
   int i;
   int intanswer[64];
   char answer[64];

 char strhex[100] = {"0201002258595A5F436F7270537570657220547572626F204469736B3230333435383933343501020080"};
 char strascii[100] = {"0883003102010022IBM CorpNot a disk      35354345345345345345345345"};
// char strhex[100] = {"010000000000000000000000000000000000000000000000000000000000000000000000000000000000"};

 
 printf("Starting the program\n");
  if (argc < 2)
 {
     printf("Usage : ascii <strhex | strascii>\n");
     exit(0);
 }
 
 if (strcmp(argv[1], "strhex") == 0)
 {
     printf("Copying hex string\n");
     memcpy(str, strhex, 64);
 }
 else if (strcmp(argv[1], "strascii") == 0)
 {
     printf("Copying ascii string\n");
     memcpy(str, strascii, 64);
 }
 else
 {
     printf("Usage : ascii <strhex | strascii>\n");
     exit(0);
 }
 str[63] = '\0';


    strType = str[3];
    printf("StrType is %c\n", strType);
    if ((strType == '2') || (strType == '3') )
    {
        printf("StrType is 2 or 3\n");
        memcpy(localBuf,  str+8, 56);
    }
    else if (strType == '1') 
    {
        printf("strtype is 1 or 0\n");
        memcpy(hexBuf, str+8, 56);
        printf("Converting hex to ascii\n");
        sprintf(localBuf, "%s", Hex2Asc(hexBuf));
    }
    else
    {
        for (i = 0; i <63; i++)
        {
            localBuf[i] = '0';
        }
    }
 
    memcpy(returnBuf, localBuf, 64);
    returnBuf[63] = '\0';
     printf("Returning %s\n", returnBuf);
     
     /* My test code */
     i = 8;
     strType = str[i];
    while (strType != '\0')
    {
        printf("Assigning character %c\n", str[i]);
        answer[8-i] = toascii(str[i]);
        i++;
        strType = str[i];
    }
    answer[63] = '\0';
    printf("Answer is %s\n", answer);

 return;
}

// printf("\nCheck is %s\n", Hex2Asc("537570657220547572626F204469736B"));
// printf("%s\n", Hex2Asc("4578616D706C65")); /*Example*/
// printf("%s\n", Hex2Asc("537570657220547572626F204469736B")); /*isDigit*/
// printf("%s\n", Hex2Asc("088300320201002258595A5F436F7270537570657220547572626F204469736B"));
// printf("%s\n", Hex2Asc(str));
// printf("%s\n", Hex2Asc("58595A5F436F7270537570657220547572626F204469736B"));

