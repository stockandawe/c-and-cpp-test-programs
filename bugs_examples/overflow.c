#include <stdio.h>

int main()
{
	int a = -10;
	size_t b = 10;

	int c = a/b; //what is c? It's 429496728, not -1! WHY????
	printf("size of int is %i and size of size_t is %i\n", 
		sizeof(int), sizeof(size_t));
	printf("c is now %i\n", c);

	return 1;
}
