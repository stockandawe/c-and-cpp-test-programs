#include <stdio.h>
#include <string.h>

#define PASSWORD "secret"

int password_compare(char *a, char *b)
{
  /* password compare logic */
  if (!strcmp(a, b))
  	return 1;
  return 0;
}
int password (char *buf)
{
 char var[16];
 strcpy(var,buf);
 if (password_compare(var, PASSWORD))
   return 1;

 return 0;
}

main (int argc, char **argv)
{
 if (1 == password(argv[1]))
 {
     printf("Congratulations, right password\n");
     /*
     * User authenticaed, do work here
     */
     return;
 }

 printf("Access Denied, wrong password!\n");
 /*
  * User login unsuccessful!
  * Exit
  */
}
