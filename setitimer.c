/*
 * Test program that checks how setitimer works. 
 * When using ITIMER_REAL, it decrements in real time 
 * and delivers SIGALRM upon expiration. 
 * We run a while() loop that decrements cnt each time 
 * the SIGALRM happens. 
 */

#include <sys/time.h>
#include <signal.h>
#include <stdio.h>
#include <assert.h>

volatile int cnt=100;

void sigalrm(int sig) {
    extern volatile int cnt;
    cnt--;
    if (!(cnt % 10)) fprintf(stderr,"%d...", cnt);
}

int main(int argc, char* argv[])
{
    struct sigaction sa = {sigalrm, 0, 0, 0};
    struct itimerval iv = {{0, 100000}, {0, 100000}}; /* 0.1 s periodic */

    assert(-1 != sigaction(SIGALRM, &sa, NULL));
    assert(-1 != setitimer(ITIMER_REAL, &iv, NULL));
    
    fprintf(stderr,"cnt=%d...", cnt);
    
    do /* nothing */; while (cnt);
    fprintf(stderr,"done - exit\n");
    return 0;
}

