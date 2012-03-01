#include <stdio.h>

void overflow (char *buf)
{
 char var[16];
 strcpy(var,buf);
}

main (int argc, char **argv)
{
 overflow(argv[1]);
 printf("no hack 1");
 printf("no hack 2");
 printf("no hack 3");
 printf("no hack 4");
 printf("no hack 5");
 printf("hack");
}
