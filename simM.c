#include <stdio.h>
#include "simM.h"

void main(argc, argv)
{
    char    dirname[128];
    int     num_of_nodes;
 
    if (argc < 2)
    {
        printf("usage: simM <num_of_nodes> <dirname>\n");
        exit(-1);
    }

    printf("Bootstrapping the network.....\n");
    
    /* get number of nodes */
    num_of_nodes = atoi(argv[0]);

    /* save the directory name */
    strcpy(dirname, argv[1]);
    printf("The dirname is %s\n", dirname);
}
