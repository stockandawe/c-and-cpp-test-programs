#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
//#include "node_process.h"

/*
 * Global defines
 */
#define BACKLOG                         10   //how many pending connections 
                                             //queue will hold

/*
 * Global variables
 */

int main(int argc, char ** argv)
{
    int     simM_port; /* port that simM is bind() on */
    int     node_identifier; /* node_identifier of this node_process */
    char    simM_hostname[128]; /* hostname/ip-addr of simM */
    int     sockfd;      /* socket that simM uses */
    struct  sockaddr_in serv_addr;   /* address of simM */
    int     node_process_port; /* port that this process is bind() on */
    char    msg[256];   /* buffer used to send to simM */
#if 0
    int     recv_fd;  /* this is the FD that you use for accept */
    struct  sockaddr_in sender_addr;  /* whenever you get a message, sender_addr is  this */
    int     sin_size; /* size needed for listen */
#endif

    if (argc < 3)
    {
        printf("usage: node_process <node_id> <simM_hostname> <simM_port>\n");
        exit(-1);
    }

    sleep(5);
    printf("node_process started.....\n");

    /* get number of nodes */
    node_identifier = atoi(argv[1]);
    printf("Node Identifier = %d\n", node_identifier);

    /* get the hostname/ip-addr of simM */
    strcpy(simM_hostname, argv[2]);
    printf("The simM_hostname is %s\n", simM_hostname);

    /* get port of simM */
    simM_port = atoi(argv[3]);
    printf("simM_port = %d\n", simM_port);

    /*
     * Step 1: Create socket(), bind() to send it's port
     * to simM.
     */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        printf("ERROR opening socket: %s\n", strerror(errno));
        exit(-1);
    }

    /* 
     * The node_process_port is going to be 2001N where N is the 
     * node_identifier. So, we set this here, to send it to simM
     */
    node_process_port = 20010 + node_identifier;
    serv_addr.sin_family = AF_INET;
    /* 
     * TODO: simM_hostname should be an IP-address otherwise connect() 
     * does not work properly.
     */
    serv_addr.sin_addr.s_addr = inet_addr("10.3.0.104"); 
    serv_addr.sin_port = htons(simM_port);/* the port that simM uses */
    memset(&(serv_addr.sin_zero), '\0', 8); /* zero rest of the struct */


    /* connect to simM so that you can send it the message */
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) < 0)
    {
        printf("ERROR in connect: %s\n", strerror(errno));
        exit(-1);
    }

    /* create the message to send to simM  */
    sprintf(msg, "node_port_msg: %d %d", node_identifier, node_process_port);

    /* send the message to simM */
    if (send(sockfd, msg, strlen(msg), 0) < 0)
    {
        printf("ERROR in send: %s\n", strerror(errno));
        exit(-1);
    }

    /*
     * Stay in a while loop so that we can verify this process 
     * runs on the remote machine.
     */
    while(1)
    {
        /* TODO */
    }

    /* End of program */
    return(1);
}
