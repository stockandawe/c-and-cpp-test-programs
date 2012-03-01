#include <stdio.h>

int main(int argc, char** argv)
{
    FILE *fp;
    long count, i;

    if (argc < 2)
    {
        printf("usage: createfile <num_of_bytes>");
        exit(0);
    }
    
    count = atoi(argv[1]);

    fp = fopen("/data/home/rutul/test_progs/file", "w+");

    for (i = 0; i < count*1024; i++)
    {
        fprintf(fp, "a");
    }

    fclose(fp);

    return 1;
}
