#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#define password "FindMeIfYouCan"

int main(int argc, char *argv[])
{
  char pass[100];
  
  
  printf("Please enter your password\n\n");
  scanf("%s", pass);
  if ( strcmp(pass, password) == 0 )
  {
       printf("Congrats!! Correct Pass\n\n");
  }
  else
  {
      printf("Wrong Pass\n\n");
  }
       
  return 0;
}
