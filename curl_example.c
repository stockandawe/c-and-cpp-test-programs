#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>                 /* for O_RDONLY                           */
#include "/data/home/rutul/curl-7.10.1/include/curl/curl.h"

  /*
   * This example shows an FTP upload, with a rename of the file just after
   * a successful upload.
   *
   * Example based on source code provided by Erick Nuwendam. Thanks!
   */
  
  #define LOCAL_FILE      "/data/home/rutul/AdventNet.exe"
  #define UPLOAD_FILE_AS  "while-uploading.exe"
  #define REMOTE_URL      "ftp://10.3.0.104//data/home/rutul/"  UPLOAD_FILE_AS
  #define RENAME_FILE_TO  "renamed-and-fine.exe"
  
  int main(int argc, char **argv)
  {
    CURL *curl;
    CURLcode res;
    FILE *ftpfile;
    FILE * hd_src ;
    int hd ;
    struct stat file_info;
  
    struct curl_slist *headerlist=NULL;
    char buf_1 [] = "RNFR " UPLOAD_FILE_AS;
    char buf_2 [] = "RNTO " RENAME_FILE_TO;
  
    /* get the file size of the local file */
    hd = open(LOCAL_FILE, O_RDONLY) ;
    fstat(hd, &file_info);
    close(hd) ;
  
    /* get a FILE * of the same file, could also be made with
       fdopen() from the previous descriptor, but hey this is just 
       an example! */
    hd_src = fopen(LOCAL_FILE, "rb");
  
    /* In windows, this will init the winsock stuff */
    curl_global_init(CURL_GLOBAL_ALL);
  
    /* get a curl handle */
    curl = curl_easy_init();
    if(curl) {
      /* build a list of commands to pass to libcurl */
//      headerlist = curl_slist_append(headerlist, buf_1);
//      headerlist = curl_slist_append(headerlist, buf_2);
  
      /* enable uploading */
      curl_easy_setopt(curl, CURLOPT_UPLOAD, TRUE) ;

      /* set user name and password */
      curl_easy_setopt(curl, CURLOPT_USERPWD, "rutul:topspin");

      /* specify target */
      curl_easy_setopt(curl,CURLOPT_URL, REMOTE_URL);
  
      /* pass in that last of FTP commands to run after the transfer */
//      curl_easy_setopt(curl, CURLOPT_POSTQUOTE, headerlist);
  
      /* now specify which file to upload */
      curl_easy_setopt(curl, CURLOPT_INFILE, hd_src);
  
      /* and give the size of the upload (optional) */
      curl_easy_setopt(curl, CURLOPT_INFILESIZE, file_info.st_size);
  
      /* Switch on full protocol/debug output */
      curl_easy_setopt(curl, CURLOPT_VERBOSE, TRUE);

      /* Now run off and do what you've been told! */
      res = curl_easy_perform(curl);
  
      /* clean up the FTP commands list */
      curl_slist_free_all (headerlist);
  
      /* always cleanup */
      curl_easy_cleanup(curl);
    }
    fclose(hd_src); /* close the local file */
  
   curl_global_cleanup();
   return 0;
 }
