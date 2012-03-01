#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#if 1
int
main()
{

pid_t ch_pid;
int ch_stat, status;
//void exit();

    printf("Program to check the use of exec(shell)\n");
	
    if ((ch_pid = fork()) < 0) 
    {

		/* Could not fork... check errno */

	}
	else if (ch_pid == 0) /* child */
    {			
		(void)execl("/usr/bin/telnet", "prog", "127.0.0.1", "2464", (char *)NULL);
        printf("In Child\n");
		exit(2);	/* execl() failed */
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

    return 1;
}
#endif

#if 0
int main()
{
    printf("Program to check exec of shell\n");

    return 1;
}
#endif
