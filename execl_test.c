#include <stdio.h>

int main()
{
    char    cmdBuf[128];
    char    node_process_arg1[128];
    char    node_process_arg2[128];
    char    node_process_arg3[128];

        sprintf(cmdBuf, "\"ssh -l root %s /data/home/rutul/test_progs/simM/node_process\"", 
            "10.3.0.100");

        sprintf(node_process_arg1, "\"%d\"", 1);
        sprintf(node_process_arg2, "\"%s\"", "anakin");
        sprintf(node_process_arg3, "\"%d\"", 20010);

#if 0
        printf("doing execl(%s, %s , %s , %s, 0)\n", 
            cmdBuf, 
            node_process_arg1, 
            node_process_arg2, 
            node_process_arg3);
#endif

        system("/usr/bin/ssh -l root 10.3.0.100 ls -l");
//        execl("ssh -l root 10.3.0.100 /data/home/rutul/test_progs/simM/node_process", "test", "1", "anakin", "20010", 0);
#if 0
        execl(cmdBuf, 
            node_process_arg1, 
            node_process_arg2, 
            node_process_arg3, 
            0);
#endif
//        execl("/bin/ls", "ls", "-l", 0);

    return 1;
}
