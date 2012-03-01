#include <stdio.h>
#include <string.h>

/*
 * Code to describe how to define/declare a function 
 * that accepts a char pointer as argument and returns 
 * an array of integers.
 */
#if 1
int array[100];
int (*fun1(char *c))[]
{
    int i;
    for (i = 0; i < 100; i ++)
    {
        array[i] = i;
    }
    return(&array);
}
int main()
{
    int i;
    fun1("abcd");
    for (i = 0; i < 100; i ++)
    {
        printf("i is %d\n", array[i]);
    }

    return 1;
}
#endif

/*
 * Would the following code compile?
 */
#if 0
int main(void)
{
    int a[]={1,2,3},i;

    for(i=0;i<3;i++)
    {
        printf("%d",*a);
        a = a+sizeof(i);
    }

    return 1;
}
#endif

/* What would be printed in the output below ? */
#if 0
int main()
{
    char s1[]="abcd";
    char s2[10];
    char s3[]="efgh";
    int i;

    i = strcmp(strcat(s3,strcpy(s2,s1)), strcat(s3,"abcd"));
    printf("%d",i);
    
    return 1;
}
#endif
/*
 * Function to understand unions
 */
#if 0
main()
{
	typedef union
	{
		int a;
		char b[10];
		float c;
	}
	Union;
	
	Union x,y = {100};
	x.a = 50;
	strcpy(x.b,"hello");
	x.c = 21.50;
	
	printf("Union x : %d %s %f \n",x.a,x.b,x.c );
	printf("Union y : %d %s %f \n",y.a,y.b,y.c);
} 
#endif
