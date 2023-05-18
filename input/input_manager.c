#include <input_manager.h>
#include <pthread.h>

static PInputOpr g_InputDevs = NULL;

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

        }
    }
}

int InputManagerInit() {
    extern void NetinputRegister();
    NetinputRegister();
    extern void TouchscreenRegister();
    TouchscreenRegister();

    int ret;
    pthread_t tid;

    PInputOpr pTmp = g_InputDevs;
    while (pTmp) {
        ret = pTmp->DeviceInit();
        if (ret != 0) {
            return ret;
        }
        ret = pthread_create(&tid, NULL, (void *)&InputRecv, pTmp);
        pTmp = pTmp->ptNext;
    }
    return 0;
}

int main() {
    return 0;
}
