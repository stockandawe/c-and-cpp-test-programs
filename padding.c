#include <stdio.h>

struct type {
    int i;
    short j;
    char c[3];
};

struct type2 {
    char c;
    char a[3];
    int i;
    short j;
};

struct MixedData
{
    char Data1;
    char Padding0[1];
    short Data2;
    int Data3;
    char Data4;
    char Padding1[3];
};

//#pragma pack(push)
//#pragma pack(1)

struct MyPackedData
{
    char Data1;
    long Data2;
    char Data3;
};

//#pragma pack(pop)

int main()
{
    printf("Individually, int = %d, 
        short = %d, 
        char = %d\n", 
        sizeof(int), sizeof(short), sizeof(char));
    printf("size of type is %d\n", sizeof(struct type));
    printf("size of type2 is %d\n", sizeof(struct type2));
    printf("size of MixedData is %d\n", sizeof(struct MixedData));
    printf("long = %d\n", sizeof(long));
    printf("size of MyPackedData is %d\n", sizeof(struct MyPackedData));
    return 1;
}
