#include <stdio.h>

#define numrecords 10

void loop()
{
    int i;
    for( i=0;i < numrecords; i++ )
    {
        // a dozen lines of database code that used i
        i=0;
    }
}
void loop2(int x)
  {
    int i=0;
    while (1) {
      if (i >= 10) {
        if (x == 55) { // x is never updated
          break;
        }
      }
      i++;
    }
  }


int main()
{
    loop();

    return 1;
}
