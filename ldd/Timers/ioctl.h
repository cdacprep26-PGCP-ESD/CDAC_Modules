#include<stdint.h>
#ifndef GET_AVA_SPACE
#define GET_AVA_SPACE _IOR('A', 1, ulong)
#define GET_LEN _IOR('A', 2, ulong)
#define GET_CNT _IOR('A', 3, ulong)
#define SET_CNT _IOW('A', 4, int)
#define SET_INTERVAL _IOW('A', 5, int)
#endif
