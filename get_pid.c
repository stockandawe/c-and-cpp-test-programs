#include <stdio.h>
#include <unistd.h> 
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

int main()
{
    system("killall -9 tftp");
    
    return 1;
}
