#ifndef _INPUT_MANAGER_H
#define _INPUT_MANAGER_H

#include <common.h>
#include <pthread.h>

#define RING_BUFF_LEN 20
#define INPUT_BUFF_SIZE 1024

typedef struct TsInput {
    int iX;
    int iY;
    int iPressure;
} TsInput;

typedef struct NetInput {
	char str[INPUT_BUFF_SIZE];
} NetInput;

typedef struct StdInput {
	char str[INPUT_BUFF_SIZE];
} StdInput;

typedef struct KeyInput {
    int id;
    int value;
} KeyInput;

typedef union Data {
    TsInput ts;
    NetInput net;
    StdInput std;
    KeyInput key;
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

extern void ringBufferInit(RingBuffer *buffer);
extern void ringBufferExit(RingBuffer *buffer);
extern int putToBuffer(RingBuffer* buffer, InputEvent* event);
extern int getFromBuffer(RingBuffer* buffer, InputEvent* event);

extern int InputManagerInit();
extern void InputManagerExit();
extern void RegisterInput(PInputOpr pInputOpr);
extern int GetInputEvent(InputEvent* event);
extern int getBuffSize(RingBuffer* buffer);

#endif
