#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netinet/ip_icmp.h>
#include <sysexits.h>

int main()
{
    char    buf[256];
    int     i;
    pid_t ch_pid;
    int ch_stat, status;

    if ((ch_pid = fork()) < 0) 
    {

		/* Could not fork... check errno */

	}

    else if (ch_pid == 0) /* child */
    {
        for (i = 1; i < 254; i++)
        {
            sprintf(buf, "ping -c 1 10.3.%d.20", i);
    //        exec(buf);
            (void)execl(buf , "prog", NULL, NULL, (char *)NULL);

        }
    }
	else /* parent */
    {	
        while ((status = wait(&ch_stat)) != ch_pid) 
        {
			if (status < 0 && errno == ECHILD)
				break;
			errno = 0;
		}
	}

    return;
}
