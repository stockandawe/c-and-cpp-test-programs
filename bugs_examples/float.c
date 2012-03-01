#include <stdio.h>

int main(void) 
{
    float f1 = 0.3f;
    float f2 = 0.1f;
    printf("%f, %.10f, %f, %.10f\n", f1, f1, f2, f2);

    if(f1 == 3*f2)
        printf("%f == 3*%f\n", f1, f2);

    if(f1 == 3.f*f2)
        printf("%f == 3.0*%f\n", f1, f2);

    return 0;
}
