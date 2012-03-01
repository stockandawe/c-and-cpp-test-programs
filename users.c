#include <stdio.h>

int main()
{
    char Buf[256];

    printf("Users test program....\n");
    sprintf(Buf, "%s %s", "/usr/sbin/adduser -d /data/home/rutul/users/aaa/", "aaa");
    system(Buf);
    
}
