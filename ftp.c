#include <stdio.h>
#include "ftplib.h"

char*
_getDirFromPath(char *dirName, char *path)
{
    char *tokPtr = NULL;
    int  tokSize;
    int  pathSize;
    char localPath[100];

    strcpy(localPath, path);

    pathSize = strlen(localPath);
    tokPtr = (char *)strrchr(localPath, '/');
    tokSize = strlen(tokPtr);

    strncpy(dirName, localPath, pathSize-tokSize);
    dirName[pathSize-tokSize] = '\0';

    return dirName;
}

int main()
{
    int status = 1;
    netbuf *test;
    char dirpath[100];
    char inputFile[100] = {"/data/home/rutul/AdventNet.exe"};
    
    printf("Starting the ftp test program\n");

    FtpInit();


    status = FtpConnect("10.3.0.94:3086", &test);

    if (status > 0)
        printf("Amazing ..this actually works \n");
    else
    {
       printf("Dude...there is a problem\n");
       exit(0);
    }
    
    status = FtpLogin("rutul", "Mulder61", test);

    if (status > 0)
        printf("User logged in \n");
    else
    {
        printf("Ah...I don't think this user logged in dude\n");
        exit(0);
    }

    printf("Getting large file\n");
    status = FtpGet("/data/home/rutul/BBB.img", inputFile, FTPLIB_IMAGE, test);
//    status = FtpPut("/data/home/rutul/AdventNet.exe", "/data/home/edwin/fff.exe", 
//       FTPLIB_ASCII, test);

    if (status > 0)
    {
        printf("FtpGet done\n");
    }
    else
    {
        printf("Ftp Get failed\n");
    }
#if 0
    status = FtpChdir(inputFile, test);
    if (status > 0)
    {
        printf("Remote file is a directory\n");
        strcpy(dirpath, inputFile);
    }
    else
    {
        printf("Remote filename is a file\n");
        _getDirFromPath(dirpath, inputFile);
        printf("Got dirpath = %s\n", dirpath);
    }


    strcat(dirpath, "/BIGCHECK/");
    printf("Check access right by creating directory %s\n", dirpath);

    status = FtpMkdir(dirpath, test);
    if (status <= 0)
        printf("Access rights failed\n");
    else
    {
        printf("Access succeded\n");
        status = FtpDelete(dirpath, test);
    }
#endif

    FtpQuit(test);
    return status;
}
