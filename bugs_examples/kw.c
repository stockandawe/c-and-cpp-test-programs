#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#if 0
bool check_index_lower_legality (int index, int offset)
/**
 * This function checks that the index with the offset isn't 
 * below zero. 
 * If it is - returns 0 ;
 * If isn't - returns 1 ;
 **/
{

if (  (index + offset )<0) {
   return 0;
  }
 return 1 ; 
}
#else
bool check_index_lower_legality (int index, int offset)
/**
 * This function checks that the index with the offset isn't 
 * below zero. 
 * If it is - returns 0 ;
 * If isn't - returns 1 ;
 **/
{
 if (index <=0) {
  return 0;
 }
 return 1;
}
#endif

int main(int argc, char **argv)
{
	int oidsp[64];
	int specProb = 15;
	int len;

	if (argc != 2)
	{
		printf("Usage is %s <len>\n", argv[0]);
		return -1;
	}
	len = atoi(argv[1]);

	if (check_index_lower_legality (len,-1))
	{
		printf("if check successed\n");
		oidsp[len-1] = specProb;
	}

	return 1;
}

