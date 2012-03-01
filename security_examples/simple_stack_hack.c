#include <stdio.h>

void func(int a, int b)
{
 int * p; /* pointer to point return address */
 printf("The address of a on the stack is %x\n", &a);
 printf("The address of b on the stack is %x\n", &b);
 p = &b ; /* p points to the address of b */
 p -= 2 ; /* sub 8 bytes to from p (or &b) so */
          /* it points to the return address */
 printf("The return address is %x \n",*p);
 (*p)+= 10; /* add 10 bytes to the return address
             * to skip the num = 1 */
 printf("The new return address is %x \n",*p);
}
main()
{
 int num;
 num = 10;
 func(1,2);
 num += 1;
 num += 5;
 printf(" Num is now %d \n",num);
}
