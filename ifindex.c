#include <stdio.h>
//#include "/data/home/rutul/wrk_rel10/all/common/include/ifindex.h"

#define TS_NUMOF_PORTBITS               6
#define TS_NUMOF_SLOTBITS               5
#define TS_PORTNUM_MASK                 ((0x1 << TS_NUMOF_PORTBITS) - 1) 
#define TS_SLOTNUM_MASK                 ((0x1 << TS_NUMOF_SLOTBITS) - 1) 
#define TS_PHYS_IFINDEX_CODE            0x00
#define TS_NUMOF_PHYSBITS               11	


#define TS_GET_SLOT(ifindex)		\
	(((ifindex) >> TS_NUMOF_PORTBITS) & TS_SLOTNUM_MASK)

#define TS_GET_PORT(ifindex)		((ifindex) & TS_PORTNUM_MASK)



int main()
{
    printf("IfIndex is %d/%d\n", TS_GET_SLOT(961), TS_GET_PORT(961));

    return 1;
}    
