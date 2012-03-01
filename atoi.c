#include <stdio.h>
#include <string.h>

/*
 * simplest way I can think of doing this:
 * abcd = d*1 + c*10 + b*100 + a*1000
 * The my_atoi_backwards() checks for pCur being a valid number
 */
int simple_atoi(char *pStr)
{
    int value = 0;
    int i = 1;
    char *pCur = pStr;

    while (*pCur != '\0')
        pCur++;
    pCur--;

    while (pCur >= pStr)
    {
        value += (*pCur - '0')*i;
        i *= 10;
        pCur--;
    }
    return value;
}

int my_atoi_backwards(char *pStr)
{
    int iRetVal = 0; 
  int iTens = 1;
 
  if ( pStr )
  {
    char* pCur = pStr; 
    while (*pCur) 
      pCur++;
 
    pCur--;
 
    while ( pCur >= pStr && *pCur <= '9' && *pCur >= '0' ) 
    { 
      iRetVal += ((*pCur - '0') * iTens);
      pCur--; 
      iTens *= 10; 
    }
  } 
  return iRetVal;
}

/*
 * More complicated than doing it backwords.
 * abcd -> 0 -> a -> ab -> abc -> abcd
 * a = a + 0
 * ab = a*10 + b
 * abc = ab*10 + c
 * abcd = abc*10 + c
 */
int my_atoi_forward(char *pStr)
{
    int ret_val = 0; 
    while (*pStr)
    {
        ret_val = (*pStr - '0') + (ret_val * 10);
        pStr++;
    }
    return ret_val;
}

int main(int argc, char **argv)
{
    char src_str[5];

    if (argc < 2)
    {
        printf("usage: my_atoi num\n");
        return -1;
    }
    strcpy(src_str, argv[1]);
    printf("the source str is %s\n", src_str);
//    printf("The int value of the string is %d\n", my_atoi_forward(src_str));
    printf("The int value of the string is %d\n", simple_atoi(src_str));

    return 1;
}
