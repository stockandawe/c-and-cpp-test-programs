#include <stdio.h>
#include <sys/stat.h>
#include "/data/home/rutul/curl-7.10.1/include/curl/curl.h"

int main()
{
    CURL *curl;
    CURLcode res;
    FILE *dstFile, *srcFile;
    struct stat file_info;
    char buf_1 [] = "RNFR ftp://10.3.0.103//data/home/rutul/test_progs/tmp/"; 
    char buf_2 [] = "RNTO ftp://10.3.0.103//data/home/rutul/test_progs/tmp/BIG.tar";
    struct curl_slist *headerlist=NULL;

    
    srcFile = fopen("/data/home/rutul/libfxp-0.1.4.tar", "r");
    stat("/data/home/rutul/libfxp-0.1.4.tar", &file_info);

    curl_global_init(CURL_GLOBAL_DEFAULT);

    curl = curl_easy_init();
    if (!curl)
    {
        printf("easyhandle creation failed \n");
        return 0;
    }
    headerlist = curl_slist_append(headerlist, buf_1);
    headerlist = curl_slist_append(headerlist, buf_2);
 
    /* enable uploading */
    curl_easy_setopt(curl, CURLOPT_UPLOAD, TRUE);

    /* set user name and password */
    curl_easy_setopt(curl, CURLOPT_USERPWD, "rutul:topspin");

    /* specify remote url */
    curl_easy_setopt(curl, CURLOPT_URL, "ftp://10.3.0.103//data/home/rutul/test_progs/tmp/");
    
    /* specify the commands to perform the operation */
    curl_easy_setopt(curl, CURLOPT_POSTQUOTE, headerlist);

    /* specify file to upload */
    curl_easy_setopt(curl, CURLOPT_INFILE, srcFile);

    /* and give the size of the upload (optional) */
    curl_easy_setopt(curl, CURLOPT_INFILESIZE, file_info.st_size);

    /* Switch on full protocol/debug output */
    curl_easy_setopt(curl, CURLOPT_VERBOSE, TRUE);

    /* Perform the operation */
    res = curl_easy_perform(curl);

    /* Clean up header list */
    curl_slist_free_all (headerlist);
    /* Clean up  */
    curl_easy_cleanup(curl);

    if(CURLE_OK != res) {
      /* we failed */
      fprintf(stderr, "curl told us %d\n", res);
    }

    return 1;
}
