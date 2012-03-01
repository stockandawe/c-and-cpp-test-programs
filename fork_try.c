#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
//#include <iostream.h>
//#include <fstream.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sysexits.h>
#include<string.h>
//#include <vector.h>

//using namespace std;

int N,i;

struct /* config_file */
{
	char domain_name [30];
	int loc_x;
	int loc_y;
	int temp_1;
	int temp_2;		
	// dont know what to do for the query
	// dont know what to do for the query
}number_of_nodes;

struct number_of_nodes *NODES;
#if 0
for (i=0;i<N ;i++)
{
	NODES[i].domain_name="--";
	NODES[i].loc_x="";
	NODES[i].loc_y="";
	NODES[i].temp_1="";
	NODES[i].temp_2="";
// dont know what to do for the query
//dont know what to do for the query



}
#endif

void error(char *msg)
{
  perror(msg);
  exit(1);
}
int main(int argc, char *argv[])	//argc is the number of nodes and *argv[] will be the directory name
{
  int sockfd, newsockfd, portno, clilen; //tportno,pid
  char buffer[256];
  struct sockaddr_in serv_addr, cli_addr;
  int n; //in case of hard coding the port no add it here and remove till line 60
  int length;
//  int i=0;
  pid_t pid;

  if (argc < 2) {
    fprintf(stderr,"ERROR, no port provided\n");
    exit(1);
  }
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) 
    error("ERROR opening socket");
  bzero((char *) &serv_addr, sizeof(serv_addr));  //char sim_port[5];
  portno = atoi(argv[1]);	

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);
  if (bind(sockfd, (struct sockaddr *) &serv_addr,
	   sizeof(serv_addr)) < 0) 
    error("ERROR on binding");
  listen(sockfd,5);
  clilen = sizeof(cli_addr);


  /* 
   * NOTE:
   * If you use the accept() statement here, what wil happen is that 
   * till you receive something on the socket (witht the accept() command), 
   * you will be blocked at this statement and any of the code below
   * won't be executed. 
   * Hence, you should put this accept() statement parent process after 
   * all the fork() has been done.
   */
#if 0
  newsockfd = accept(sockfd, 
		     (struct sockaddr *) &cli_addr, 
		     (socklen_t*)&clilen);

#endif


  if(getsockname(sockfd, (struct sockaddr*)&serv_addr, (socklen_t*)&length)<0)
  {
      error("ERROR on getsockname");
  }
 
   //************************************//
   //*********forking nodes*************//
   //***********************************//
    /*
     * Test Code: We will fork 5 processes.
     */
    N = 5;

      for (i = 0; i <= N; i++)	//N is the number of nodes
      {
        printf("Forking child %d\n", i);
	    pid = fork();
		if(pid < 0)
		error ("ERROR on fork");
        if (pid == 0)
        {
			 sleep(5);
			 char client_id[5],trport[5],crport[5],num[3];
//			 sprintf(trport, "%d", tport); 
	      
//			 sprintf(crport, "%d", cport); 
			 sprintf(client_id,"%d",i);
			 printf("Hello, I am child node #%d with pid = %d\n", i, getpid());
// 	         execl("./node", "NODE", mrport,trport,crport,client_id,NULL);
             // execl("./client3", "client3", mrport,trport,crport,client_id,NULL);
             while(1)
             {
                 /* 
                  * This is the child process. Each child process will have a new process id. 
                  * You can know that bythe getpid() command as I have used in the printf.
                  * I am using this while() loop because otherwise even the child process 
                  * will loop and go into the for() procedure and fork() new processes. 
                  * This way, each child process stays in this loop till you press 
                  * control-C
                  */
             }
        } //end of if
        else 
        {
            /*
             * This is the parent process. In this, after all the "nodes" are 
             * done with fork(), you will have to use wait() or something similar 
             * to check when the child processes are done executing.
             */
        
        } //end of else
        

      } //end of for
    
} 


#if 0
  if (newsockfd < 0) 
    error("ERROR on accept");
  bzero(buffer,256);
  n = read(newsockfd,buffer,255);
  if (n < 0) error("ERROR reading from socket");
  printf("Here is the message: %s\n",buffer);
  n = write(newsockfd,"I got your message",18);
  if (n < 0) error("ERROR writing to socket");
  return 0; 



}

#endif
