#ifndef _INPUT_MANAGER_H
#define _INPUT_MANAGER_H

#include <common.h>
#include <string.h>
#include <pthread.h>

#define RING_BUFF_LEN 20

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

typedef struct InputOpr {
    char* name;
    int (*GetInputEvent)(PInputEvent ptInputEvent);
    int (*DeviceInit)(void);
    int (*DeviceExit)(void);
    struct InputOpr* ptNext;
} InputOpr, *PInputOpr;

typedef struct RingBuffer {
    int len;
    int read;
    int write;
    InputEvent inputEvents[RING_BUFF_LEN];
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} RingBuffer;

extern RingBuffer gRingBuffer;
extern void RegisterInput(PInputOpr pInputOpr);
extern void ringBufferInit(RingBuffer *buffer);
extern int putToBuffer(RingBuffer* buffer, InputEvent* event);
extern int getFromBuffer(RingBuffer* buffer, InputEvent* event);

#endif
