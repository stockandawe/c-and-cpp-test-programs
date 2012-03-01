#include <stdio.h>
#include "/data/home/rutul/curl-7.10.1/include/curl/curl.h"
//#include "/data/home/rutul/curl-7.10.1/include/curl/mprintf.h"
//#include "/data/home/rutul/curl-7.10.1/include/curl/multi.h"
//#include "/data/home/rutul/curl-7.10.1/include/curl/types.h"
//#include "/data/home/rutul/curl-7.10.1/include/curl/stdcheaders.h"
//#include "/data/home/rutul/curl-7.10.1/include/curl/easy.h"

int write_data(void *buffer, size_t size, size_t nmemb, void *data)
{
    FILE *fp = (FILE *)data;
    if (fp == NULL)
        return;
    return fwrite(buffer, size, nmemb, fp);
}

struct FtpFile {
  char *filename;
  FILE *stream;
};

int my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream)
{
  struct FtpFile *out=(struct FtpFile *)stream;
  if(out && !out->stream) {
    /* open file for writing */
    out->stream=fopen(out->filename, "wb");
    if(!out->stream)
      return -1; /* failure, can't open file to write */
  }
  return fwrite(buffer, size, nmemb, out->stream);
}



int main()
{
    CURL *easyhandle;
    CURLcode res;
    FILE *file;
    struct FtpFile ftpfile={
        "/data/home/rutul/test_progs/CURLFTP", /* name to store the file as if succesful */
        NULL
      };
    
    file = fopen("/data/home/rutul/test_progs/CURLFTP", "wb");
    printf("Starting curl_ftp program\n");
    
    curl_global_init(CURL_GLOBAL_DEFAULT);

    easyhandle = curl_easy_init();
    if (!easyhandle)
    {
        printf("easyhandle creation failed \n");
        return 0;
    }
    
    res = curl_easy_setopt(easyhandle, CURLOPT_USERPWD, "rutul:topspin");
    printf("res value is = %d\n", res);
    res = curl_easy_setopt(easyhandle, CURLOPT_URL, "ftp://10.3.0.103//data/home/rutul/ftpfile.cfg");
    printf("res value is = %d\n", res);

#if 0
    /* Define callback to get called when there is data to be written */
    curl_easy_setopt(easyhandle, CURLOPT_WRITEFUNCTION, my_fwrite);

    /* Set a pointer to our struct to pass to the callback */
    curl_easy_setopt(easyhandle, CURLOPT_FILE, &ftpfile);
#endif 
#if 1
    /* Define callback to get called when there is data to be written */
    res = curl_easy_setopt(easyhandle, CURLOPT_WRITEFUNCTION, write_data);
    printf("res value is = %d\n", res);

    /* Set a pointer to our struct to pass to the callback */
    res = curl_easy_setopt(easyhandle, CURLOPT_FILE, NULL);
    printf("res value is = %d\n", res);
#endif
   
    /* Switch on full protocol/debug output */
    curl_easy_setopt(easyhandle, CURLOPT_VERBOSE, TRUE);

    /* Perform the operation */
    res = curl_easy_perform(easyhandle);

    /* Clean up  */
    curl_easy_cleanup(easyhandle);

    switch(res)
     {
            case CURLE_OK:
                printf("Operation Successful\n");
                break;
                
            case CURLE_COULDNT_RESOLVE_HOST:
                printf("Destination not reachable\n");
                break;
                
            case CURLE_COULDNT_CONNECT:
                printf("Couldn't connect\n");
                break;
                
            case CURLE_FTP_WEIRD_SERVER_REPLY:
                printf("weird server reply\n");
                break;

            case CURLE_FTP_ACCESS_DENIED:
                printf("access denied\n");
                break;

            case CURLE_FTP_USER_PASSWORD_INCORRECT:
                printf("user password incorrect\n");
                break;

            case CURLE_FTP_CANT_GET_HOST:
                printf("ftp can't get host\n");
                break;

            case CURLE_FTP_COULDNT_SET_BINARY:
                printf("ftp could not set binary\n");
                break;

            case CURLE_FTP_COULDNT_RETR_FILE:
                printf("ftp file does not exists\n");
                break;

            default:
                printf("FAILED\n");
                break;

     } /* end of switch() */

    easyhandle = curl_easy_init();
    if (!easyhandle)
    {
        printf("easyhandle creation failed \n");
        return 0;
    }
    
    res = curl_easy_setopt(easyhandle, CURLOPT_USERPWD, "rutul:topspin");
    printf("res value is = %d\n", res);
    res = curl_easy_setopt(easyhandle, CURLOPT_URL, "ftp://10.3.0.103//data/home/rutul/ftpfile.cfg");
    printf("res value is = %d\n", res);

    if(CURLE_OK != res) {
      /* we failed */
      fprintf(stderr, "curl told us %d\n", res);
    }

    /* Define callback to get called when there is data to be written */
    res = curl_easy_setopt(easyhandle, CURLOPT_WRITEFUNCTION, write_data);
    printf("res value is = %d\n", res);

    /* Set a pointer to our struct to pass to the callback */
    res = curl_easy_setopt(easyhandle, CURLOPT_FILE, file);
    printf("res value is = %d\n", res);

    /* Switch on full protocol/debug output */
    curl_easy_setopt(easyhandle, CURLOPT_VERBOSE, TRUE);

    /* Perform the operation */
    res = curl_easy_perform(easyhandle);

    /* Clean up  */
    curl_easy_cleanup(easyhandle);

    return 1;
}

