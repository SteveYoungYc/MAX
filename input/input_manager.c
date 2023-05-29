#include <input_manager.h>
#include <input/netinput.h>
#include <input/stdinput.h>

static PInputOpr g_InputDevs = NULL;
RingBuffer gRingBuffer;

void RegisterInput(PInputOpr pInputOpr) {
    pInputOpr->ptNext = g_InputDevs;
    g_InputDevs = pInputOpr;
}

void InputRecv(void* data) {
    PInputOpr pOpr = (PInputOpr)data;
    InputEvent event;
    int ret;
    while (1) {
        ret = pOpr->GetInputEvent(&event);
        if (!ret) {
            pthread_mutex_lock(&gRingBuffer.mutex);
            putToBuffer(&gRingBuffer, &event);
            pthread_cond_signal(&gRingBuffer.cond);
            pthread_mutex_unlock(&gRingBuffer.mutex);
        }
    }
}

int InputManagerInit() {
    NetinputRegister();
    StdinputRegister();

    int ret;
    pthread_t tid;
    PInputOpr pTmp;

    ringBufferInit(&gRingBuffer);

    pTmp = g_InputDevs;
    while (pTmp) {
        ret = pTmp->DeviceInit();
        if (ret != 0) {
            return ret;
        }
        ret = pthread_create(&tid, NULL, (void*)&InputRecv, pTmp);
        pTmp = pTmp->ptNext;
    }
    return 0;
}

void InputManagerExit() {
    ringBufferExit(&gRingBuffer);
}

int GetInputEvent(InputEvent* event) {
    int ret;
    pthread_mutex_lock(&gRingBuffer.mutex);
    if (!getFromBuffer(&gRingBuffer, event)) {
        pthread_mutex_unlock(&gRingBuffer.mutex);
        return 0;
    }
    pthread_cond_wait(&gRingBuffer.cond, &gRingBuffer.mutex);
    if (!getFromBuffer(&gRingBuffer, event)) {
        ret = 0;
    } else {
        ret = -1;
    }
    pthread_mutex_unlock(&gRingBuffer.mutex);
    return ret;
}
