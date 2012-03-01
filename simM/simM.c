#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include "simM.h"

/*
 * Global defines
 */
#define BACKLOG                         10   //how many pending connections 
                                             //queue will hold
#define MAXDATASIZE                     256  //maximum data to recv
#define SIMM_TCP_PORT                   20010


/*
 * Global variables
 */
char    simM_hostname[128];


void process_recv_msg(char *recv_msg)
{
    return;
}


int main(int argc, char ** argv)
{
    struct  node_info node_list[128]; /* max number of nodes is 128 */
    char    dirname[128];   /* for dirname > 128 chars, change this */
    int     num_of_nodes;
    FILE    *fp;
    char    input_line_from_file[128]; /* input from file cannot be > 128 */
    char    file_to_open[128];
    int     i;
    static int query_number = 0;
    int     sockfd;      /* socket that simM uses */
    struct  sockaddr_in serv_addr;   /* address of simM */
    int     recv_fd;  /* this is the FD that you use for accept */
    struct  sockaddr_in sender_addr;  /* whenever you get a message, sender_addr is  this */
    int     sin_size; /* size needed for listen */
    struct hostent *h;

    if (argc < 2)
    {
        printf("usage: simM <num_of_nodes> <dirname>\n");
        exit(-1);
    }

    printf("Bootstrapping the network.....\n");

    /* get number of nodes */
    num_of_nodes = atoi(argv[1]);
    printf("Number of nodes is %d\n", num_of_nodes);
    if (num_of_nodes > 10)
    {
        printf("ERROR: Cannot have more than 10 nodes\n");
        exit(-1);
    }

    /* save the directory name */
    strcpy(dirname, argv[2]);
    printf("The dirname is %s\n", dirname);

    /*
     * Get IP-addr/hostname for simM. i.e the machine that simM is running on.
     */
    if (gethostname(simM_hostname, sizeof(simM_hostname)) < 0)
    {
        printf("ERROR: Cannot gethostname()\n");
        exit(-1);
    }
    h = (struct hostent *)gethostbyname(simM_hostname);
    /*
     * TODO: The following returns 127.0.0.1. Instead we want the 
     * IP-address of this machine (one on which simM is running) otherwise 
     * node_process will not be able to connect()
     */
    printf("IP-addr of simM is %s\n", inet_ntoa(*((struct in_addr *)h->h_addr)));

    /*
     * Step 1: Read the configuration files for each nodes.
     */
    for (i = 1; i <= num_of_nodes; i++)
    {
        /* 
         * this will give us the file name.
         * For example if dirname is "configfiles" then, 
         * the following statement will give us:
         * "configfile/1.cfg"
         * "configfile/2.cfg"
         * ...
         * upto "configfile/num_of_nodes.cfg"
         */
        sprintf(file_to_open, "%s/%d.cfg", dirname, i);
        fp = fopen(file_to_open, "r");

        if (fp == NULL)
        {
            /*
             * error in trying to open file for reading
             */
            printf("ERROR: Cannot open file %s\n", file_to_open);
            exit(-1);
        }


        /*
         * Reset query-number for each node
         */
        query_number = 0;

        node_list[i].node_identifier = i; /* node 1, is node-1 */
        printf("\n++++++++Node %d++++++++++++++\n", node_list[i].node_identifier);

        while(fgets(input_line_from_file, sizeof(input_line_from_file), fp) != NULL)
        {
//            printf("read line %s\n", input_line_from_file);
            if (strstr(input_line_from_file, "runs_on") != NULL)
            {
                sscanf(input_line_from_file, "runs_on %s", node_list[i].node_host);
                printf("node_host is %s\n", node_list[i].node_host);
            }

            if (strstr(input_line_from_file, "location") != NULL)
            {
                sscanf(input_line_from_file, "location %d %d", 
                    &(node_list[i].x), 
                    &(node_list[i].y));

                printf("x is %d, y is %d\n", 
                    node_list[i].x, node_list[i].y);
            }

            if (strstr(input_line_from_file, "temperature") != NULL)
            {
                sscanf(input_line_from_file, "temperature %d %d", 
                    &(node_list[i].temp_lo), 
                    &(node_list[i].temp_hi));

                /*
                 * Note: If temp_hi is not given in the file, it will be 0.
                 */
                printf("temp_lo is %d, temp_hi is %d\n", 
                    node_list[i].temp_lo, node_list[i].temp_hi);
            }

            if (strstr(input_line_from_file, "query") != NULL)
            {
                sscanf(input_line_from_file, "query %d %s %s", 
                    &(node_list[i].query_timer[query_number]), 
                    node_list[i].query[query_number], 
                    node_list[i].query_coordinates[query_number]);

                printf("query_number-%d timer is %d, query is %s %s\n", 
                    query_number, 
                    node_list[i].query_timer[query_number], 
                    node_list[i].query[query_number], 
                    node_list[i].query_coordinates[query_number]);

                /* update the query number */
                query_number++;
            }

        } // end of while loop
            
        /*
         * update the total number of queries for this node.
         */
        node_list[i].total_query = query_number;
        printf("This node has %d of queries\n", node_list[i].total_query);
    } 

    /* 
     * Step 2: Run the node_process on the remote machines. 
     * For this, we will do execl(ssh) for each node.
     * This execl(ssh) command will also send the node_process:
     * a) it's node_identifier
     * b) ip-addr of simM (saved in variable hostname
     * c) simM port-num which is SIMM_TCP_PORT
     */
    char    cmdBuf[128];
    char    node_process_arg1[128];
    char    node_process_arg2[128];
    char    node_process_arg3[128];

    for (i = 1; i <= num_of_nodes; i++)
    {
        sprintf(cmdBuf, "ssh -l root %s /data/home/rutul/test_progs/simM/node_process %d %s %d &", 
            node_list[i].node_host,
            node_list[i].node_identifier, 
            simM_hostname, 
            SIMM_TCP_PORT);
        
        printf("system(%s)\n", cmdBuf);

        system(cmdBuf);

#if 0
        sprintf(cmdBuf, "`ssh -l root %s /data/home/rutul/test_progs/simM/node_process`", 
            node_list[i].node_host);

        sprintf(node_process_arg1, "`%d`", node_list[i].node_identifier);
        sprintf(node_process_arg2, "`%s`", simM_hostname);
        sprintf(node_process_arg3, "`%d`", SIMM_TCP_PORT);

        printf("doing execl(%s %s %s %s)\n", cmdBuf, 
            node_process_arg1, 
            node_process_arg2, 
            node_process_arg3);

        execl(cmdBuf, 
            "node_process", 
            node_process_arg1, 
            node_process_arg2, 
            node_process_arg3, 
            0);
#endif
    }

    /*
     * Step 3: Create socket() and bind()
     */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        printf("ERROR opening socket: %s\n", strerror(errno));
        exit(-1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY; /* simM will use it's own IP address */
    serv_addr.sin_port = htons(SIMM_TCP_PORT);  /* the port that simM uses */
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
    {
        error("ERROR on bind(): %s\n", strerror(errno));
        exit(-1);
    }


    /*
     * Step 4: listen() and accept whatever the node_process on each node 
     * will send you.
     */
    if (listen(sockfd, BACKLOG) < 0)
    {
        printf("ERROR on listen(): %s\n", strerror(errno));
        exit(-1);
    }

    while(1)    /* main accept() loop */
    {
        int     recv_len;
        char    recv_buf[MAXDATASIZE];

        memset(recv_buf, 0, sizeof(recv_buf));
        sin_size = sizeof(struct sockaddr_in);
        recv_fd = accept(sockfd, (struct sockaddr *)&sender_addr, &sin_size);
        if (recv_fd < 0)
        {
            printf("ERROR in accept(): %s\n", strerror(errno));
            continue;
        }
        printf("simM got connection from %s\n", 
            inet_ntoa(sender_addr.sin_addr));

        for (i = 1; i <= num_of_nodes; i++)
        {
            if ((strcmp(inet_ntoa(sender_addr.sin_addr), node_list[i].node_host)) == 0)
            {
                node_list[i].node_process_recv_fd = recv_fd;
                printf("For node %d, node_process_recv_fd = %d\n", 
                    i, node_list[i].node_process_recv_fd);
            }
        }
    
        if ((recv_len=recv(recv_fd, recv_buf, MAXDATASIZE-1, 0)) <= 0) 
        {
            printf("ERROR in recv(): %s\n", strerror(errno));
            close(recv_fd);
            exit(-1);
        }
        else
        {
            recv_buf[recv_len] = '\0';
            printf("Received msg %s\n", recv_buf);
        }

#if 0
        {
        int     node_identifier;
        int     node_port;

//        if (strstr(recv_buf, "node_port_msg:") != NULL)
        {
            FILE    *output_file_pointer;
            char    output_file[128];

            printf("Received a node_port_msg");
            sscanf(recv_buf, "node_port_msg: %d %d", 
                &node_identifier, 
                &node_port);

            /*
             * Save this node_port in list.
             */
            node_list[node_identifier].listen_port_num = node_port;
#if 1
            /*
             * Write this information to output file. i.e. dirname/sim.out
             */
            sprintf(output_file, "%s/%s", dirname, "simM.out");
            output_file_pointer = fopen(output_file, "a"); /* open file in append mode */
            if (output_file_pointer == NULL)
            {
                printf("ERROR in opening output file %s\n", output_file);
                exit(-1);
            }

            fprintf(output_file_pointer, "%d %d\n", 
                node_list[node_identifier].node_identifier, 
                node_list[node_identifier].listen_port_num);
#endif
        }
        }
#endif

    } //end of while(1) loop


    /* End of program */
    close(sockfd);
    close(recv_fd);
    return(1);
}
