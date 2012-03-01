#include <stdio.h>

enum QueryType
{
    SI = 0,
    BSA = 1,
    APR = 2,
    ER = 3,
    RSEB = 4,
    SCR = 5
};
    
int main(void)
{
    enum QueryType qt;

    qt = SCR;

    printf("Value is %d\n", qt);
    return 0;
}
