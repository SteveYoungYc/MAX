#ifndef _COMMON_H
#define _COMMON_H

#include <sys/time.h>
#include <string.h>
#include <stdio.h>

#define INPUT_TYPE_TOUCH 1
#define INPUT_TYPE_NET 2
#define INPUT_TYPE_STD 3
#define INPUT_TYPE_KEY 4

#ifndef NULL
#define NULL (void*)0
#endif

typedef struct Region {
    int iLeftUpX;
    int iLeftUpY;
    int iWidth;
    int iHeigh;
} Region, *PRegion;

#endif
