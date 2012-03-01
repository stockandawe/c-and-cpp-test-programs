#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

#define CDP_TLV_CACHE_MAX_LEN   1024

int main()
{
    FILE                    *fp;
    char                    Input[12*CDP_TLV_CACHE_MAX_LEN];
    char                    deviceId[CDP_TLV_CACHE_MAX_LEN], platform[CDP_TLV_CACHE_MAX_LEN], port_id[CDP_TLV_CACHE_MAX_LEN];
    char                    version[CDP_TLV_CACHE_MAX_LEN], sysname[CDP_TLV_CACHE_MAX_LEN], ipaddr[CDP_TLV_CACHE_MAX_LEN];
    char*                   tmp;
    int                     i=0;
    int                     IfIndex, DeviceIndex, Duplex, NativeVLAN, MTU;

    fp = fopen("/tmp/cdpCache", "r");
    if (fp == NULL)
    {
        /* error in opening file for reading */
        printf("error in opening file for reading\n");
        exit(-1);
    }

    memset(Input, 0, strlen(Input));
    memset(deviceId, 0, CDP_TLV_CACHE_MAX_LEN);
    memset(platform, 0, CDP_TLV_CACHE_MAX_LEN);
    memset(port_id, 0, CDP_TLV_CACHE_MAX_LEN);
    memset(version, 0, CDP_TLV_CACHE_MAX_LEN);
    memset(sysname, 0, CDP_TLV_CACHE_MAX_LEN);
    memset(ipaddr, 0, CDP_TLV_CACHE_MAX_LEN);

    while(fgets(Input, sizeof(Input), fp) != NULL)
    {
        printf("Got string------> %s\n", Input);
     
        sscanf(Input, "%d %d %d %d %d", 
            &(IfIndex), 
            &(DeviceIndex), 
            &(Duplex), 
            &(NativeVLAN), 
            &(MTU)); 

        i = 0;
        tmp = strstr(Input, "~");
        tmp = tmp + 1;
        while (*tmp != '~')
        {
            deviceId[i] = *tmp;
            tmp = tmp + 1;
            i++;
        }

        i = 0;
        tmp = strstr(Input, tmp);
        tmp = tmp + 1;
        while (*tmp != '~')
        {
            platform[i] = *tmp;
            tmp = tmp + 1;
            i++;
        }

        i = 0;
        tmp = strstr(Input, tmp);
        tmp = tmp + 1;
        while (*tmp != '~')
        {
            port_id[i] = *tmp;
            tmp = tmp + 1;
            i++;
        }

        i = 0;
        tmp = strstr(Input, tmp);
        tmp = tmp + 1;
        while (*tmp != '~')
        {
            version[i] = *tmp;
            tmp = tmp + 1;
            i++;
        }

        i = 0;
        tmp = strstr(Input, tmp);
        tmp = tmp + 1;
        while (*tmp != '~')
        {
            sysname[i] = *tmp;
            tmp = tmp + 1;
            i++;
        }

        i = 0;
        tmp = strstr(Input, tmp);
        tmp = tmp + 1;
        while (*tmp != '~')
        {
            ipaddr[i] = *tmp;
            tmp = tmp + 1;
            i++;
        }

        printf("devceId = %s\n", deviceId);
        printf("platform = %s\n", platform);
        printf("portid = %s\n", port_id);
        printf("version = %s\n", version);
        printf("sysname = %s\n", sysname);
        printf("ipaddr = %s\n", ipaddr);
        printf("ifindex = %d\n", IfIndex);
        printf("deviceindex = %d\n", DeviceIndex);
        printf("duplex = %d\n", Duplex);
        printf("NativeVLAN = %d\n", NativeVLAN);
        printf("MTU = %d\n", MTU);

        memset(Input, 0, strlen(Input));
        memset(deviceId, 0, CDP_TLV_CACHE_MAX_LEN);
        memset(platform, 0, CDP_TLV_CACHE_MAX_LEN);
        memset(port_id, 0, CDP_TLV_CACHE_MAX_LEN);
        memset(version, 0, CDP_TLV_CACHE_MAX_LEN);
        memset(sysname, 0, CDP_TLV_CACHE_MAX_LEN);
        memset(ipaddr, 0, CDP_TLV_CACHE_MAX_LEN);

    }
    fclose(fp);
    return 1;
}

