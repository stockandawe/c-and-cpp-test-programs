#include <stdio.h>
#include <pthread.h>

/* To complile: gcc -o thread_example thread_example.c -lpthread */

void
thread1_fun(void)
{
    int oldCancelState;
    int oldCancelType;

    printf("Withing thread 1\n");
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &oldCancelState);
    printf("PTHREAD_CANCEL_ENABLE = %d, oldCancelState = %d\n", 
        PTHREAD_CANCEL_ENABLE, oldCancelState);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &oldCancelType);
    printf("oldCancelType = %d PTHREAD_CANCEL_ASYNC = %d PTHREAD_CANCEL_DEFERRED = %d\n", 
        oldCancelState, PTHREAD_CANCEL_ASYNCHRONOUS, PTHREAD_CANCEL_DEFERRED);

    while(1)
    {
        sleep(1);
        printf("awake after sleeping\n");

    }
}

main()
{
    pthread_t   thread1;
    int         ret;

    /* Create a thread */
    ret = pthread_create(&thread1, NULL, (void *)thread1_fun, NULL);

    if (ret != 0)
    {
        printf("Error in return %s\n", strerror(ret));
        exit(0);
    }

    printf("pthread_create successful\n");

    sleep(5);

    printf("main_thread awake after sleeping. doing pthread_cancel\n");
    ret = pthread_cancel(thread1);
    if (ret == 0)
    {
        printf("pthread_cancel successful\n");
    }
            
    exit(0);
}
