#include <stdio.h>
#include <netinet/in.h>

int main( void )
{
    struct in_addr  in;
    int  a, b, c, d;

    char*  ipaddr = "192.168.1.156";

    a = b = c = d = 0;

    inet_aton( ipaddr, &in );

    a = in.s_addr & 0x000000FF;
    /*
    b = in.s_addr & 0x0000FF00;
    c = in.s_addr & 0x00FF0000;
    d = in.s_addr & 0xFF000000;
    */
    b = (in.s_addr & 0x0000FF00)>>8;
    c = (in.s_addr & 0x00FF0000)>>16;
    d = (in.s_addr & 0xFF000000)>>24;
    printf( "%ld.%ld.%ld.%ld\n", a, b, c, d );
    return 0;
}

