#include <stdio.h>

#if 0
int a=0x61;
int b=0x10;
int c=0x40;
int d=0x00;
#else
int a=0x32;
int b=0x84;
int c=0x04;
int d=0x08;
#endif

int main()
{
	printf("%c%c%c%c", a, b, c, d);
	printf("\n");
	printf("%02x%02x%02x%02x", a, b, c, d);
	printf("\n");
	return 0;
}
