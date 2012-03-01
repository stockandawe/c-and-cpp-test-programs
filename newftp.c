#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <sys/stat.h>

#include "/data/home/rutul/libfxp-0.1.4/fxp.h"
int verbosity= VERBOSITY_DEBUG;

fxp_error_t the_hook(fxp_handle_t fxp, const char *message, unsigned message_len)
{
  fprintf(stderr, CYELLOW "%s" COFF, message);
  fflush(stderr);

  return FXPOK;
}

int main()
{
    url_t url;
    int status = 1;
    fxp_handle_t fxp;
    fxp_error_t fxpret;
    char dirpath[100];
//    char inputFile[100] = {"/data/home/rutul/test_progs/new.cfg"};
    strcpy(url.host, "10.3.0.104");
    
    printf("Starting the ftp test program\n");

    if(parse_url("10.3.0.104", &url) != 0) {
    error("parse_url() failed\n");
    return -1;
  }

  if(fxp_init(&fxp, &url) != FXPOK) {
    error("fxp_init() failed\n");
    return -1;
  }
  fxp_set_control_send_hook(fxp, the_hook);
  fxp_set_control_receive_hook(fxp, the_hook);
  if(fxp_connect(fxp, 10000) != FXPOK) {
    error("fxp_connect() failed\n");
    exit(0);
  }
  
  if(fxp_login(fxp, "rutul", "topspin") != FXPOK) {
    error("fxp_login() failed\n");
  }
  printf("Login successful\n");


  fxp_disconnect(fxp);
  fxp_destroy(&fxp);


    return status;
}
