#include <stdio.h>

struct node_info {
    int  node_identifier; /* id or number of the node 1, 2, ...N */
    char node_host[128]; /* this is runs_on field from the .cfg file */
    int  x;              /* x co-ordinate */
    int  y;              /* y co-odrinate */
    int  temp_lo;        /* lo value of temp */
    int  temp_hi;        /* hi value of temperature, if provided, otherwise 0 */
    int  query_timer[10];    /* if provided, miliilseconds */
    char query[10][128];     /* query of the form, "min" or "max" */
    char query_coordinates[10][128]; /* if provided, of the form [x1,y1,x2,y2] */
    int  total_query;     /* there can be more than 1 query per node */
    int  listen_port_num;/* the port number that the ports is listening on */ 
    int  node_process_recv_fd;

};

