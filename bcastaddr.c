#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <netinet/in.h>

//char* my_ntoa(struct in_addr addr)
//{

//}

int main()
{
    /*
    int addr = 0x1a1a1a1a;
    int subnetmask = 0xffff0000;
    int bcastaddr = 0x00000000;

    bcastaddr = ((addr & subnetmask) ^ (0xffffffff ^ subnetmask));

    printf("\naddr = %x, subnetmask = %x, bcastddr = %x\n\n", 
        addr, subnetmask, bcastaddr);
    */
    
    char printBuf[256];
    struct in_addr addr;
    addr.s_addr = 0x0a030001;
    strcpy(printBuf, inet_ntoa(addr));

    printf("addr is %s\n", printBuf);

    return 1;
}

