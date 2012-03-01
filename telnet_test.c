#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "/data/home/rutul/curl-7.10.1/include/curl/curl.h"

int write_data(void *buffer, size_t size, size_t nmemb, void *data)
{
    FILE *fp = (FILE *)data; 
    
    if (fp == NULL)
    {
        printf("fp = NULL");
        return 0;
    }
    return fwrite(buffer, size, nmemb, fp);
    fflush(fp);
    fclose(fp);
}


int main()
{
    CURL *easyhandle;
    CURLcode res;
    char userpasswdStr[256];
    char sourceUrl[256];
//    FILE *fp;

    printf("Starting telnet test program\n");

    /* Global initialize curl */
    res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (res == CURLE_FAILED_INIT)
    {
        printf("curl_global_init() failed\n");
        return;
    }
    
    /* Get a curl easy-handle */
    easyhandle = curl_easy_init();    
    if (!easyhandle)
    {
        printf("failed to get easy handle\n");
        return;
    }

//    /* Switch on full protocol/debug output */
    curl_easy_setopt(easyhandle, CURLOPT_VERBOSE, TRUE);

    sprintf(sourceUrl, "%s", "telnet://10.3.102.15");
    res = curl_easy_setopt(easyhandle, CURLOPT_URL, sourceUrl);

//    sprintf(userpasswdStr, "%s:%s", "root", "root");
//    res = curl_easy_setopt(easyhandle, CURLOPT_USERPWD, userpasswdStr);
#if 1

    /* Define callback to get called when there is data to be written */
    curl_easy_setopt(easyhandle, CURLOPT_WRITEFUNCTION, write_data);
    
    /* Set a pointer to our struct to pass to the callback */
//    fp = fopen(stdout, "wb");
    curl_easy_setopt(easyhandle, CURLOPT_FILE, stdout);
#endif

    /* Perform the operation */
    res = curl_easy_perform(easyhandle);
    
    /* Perform cleanup */
    curl_easy_cleanup(easyhandle);

            printf("******************Got res as %d\n", res);
            switch(res)
            {
            case CURLE_OK:
                printf("Good!!\n");
                break;

            case CURLE_WRITE_ERROR:
                printf("Houston we have a problem here\n");
                break;
                
            case CURLE_COULDNT_RESOLVE_HOST:
                printf("Couldn't resolve host\n");
                break;
                
            case CURLE_COULDNT_CONNECT:
                printf("Couldn't connect\n");
                break;
                
            case CURLE_FTP_USER_PASSWORD_INCORRECT:
                printf("Password incorrect\n");
                break;

            case CURLE_FTP_CANT_GET_HOST:
                printf("Can't get host\n");
                break;

            case CURLE_FTP_COULDNT_SET_BINARY:
                printf("Couldn't set binary\n");
                break;

            default:
                printf("Don't know what happened\n");
                break;
            } /* end of switch() */
    return;

}
