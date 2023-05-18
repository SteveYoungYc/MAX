#ifndef _INPUT_MANAGER_H
#define _INPUT_MANAGER_H

#include <common.h>

typedef struct TsInput {
    int iX;
    int iY;
    int iPressure;
} TsInput;

typedef struct NetInput {
	char str[1024];
} NetInput;

typedef union Data {
    TsInput ts;
    NetInput net;
} Data;

typedef struct InputEvent {
    int iType;
    struct timeval tTime;
    Data data;
} InputEvent, *PInputEvent;

typedef struct InputDevice {
    char* name;
    int (*GetInputEvent)(PInputEvent ptInputEvent);
    int (*DeviceInit)(void);
    int (*DeviceExit)(void);
    struct InputDevice* ptNext;
} InputDevice, *PInputDevice;

#endif
