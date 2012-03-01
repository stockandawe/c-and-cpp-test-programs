#include <stdio.h>
#include <math.h>

double power(double val, unsigned pow)
{       double ret_val = 1.0;
        unsigned i;

        for(i = 0; i < pow; i++)
                ret_val *= val;

        return(ret_val);
}

int main()
{
    double test, val;
    printf("Starting program\n");
    val = 0.0;

    for (test = 0.0; test <=27.0; test++)
    { 
        printf("Adding Value %f\n", power(2.0, test));
        val = val + power(2.0, test);
    }
    printf("Value of val is %f\n", val);
    
    return;
}
