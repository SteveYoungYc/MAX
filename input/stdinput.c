#include <input_manager.h>

static int StdinputGetInputEvent(PInputEvent ptInputEvent) {
    int recvLen;
    char stdBuf[INPUT_BUFF_SIZE];

    if (fgets(stdBuf, sizeof(stdBuf), stdin) != NULL) {
        recvLen = strcspn(stdBuf, "\n");
        stdBuf[recvLen] = '\0';
        ptInputEvent->iType = INPUT_TYPE_STD;
        gettimeofday(&ptInputEvent->tTime, NULL);
        strncpy(ptInputEvent->data.std.str, stdBuf, INPUT_BUFF_SIZE);
        ptInputEvent->data.std.str[INPUT_BUFF_SIZE - 1] = '\0';
        return 0;
    } else {
        return -1;
    }
}

static int StdinputDeviceInit(void) {
    return 0;
}

static int StdinputDeviceExit(void) {
    return 0;
}

static InputOpr g_tStdinputDev = {
    .name = "std",
    .GetInputEvent = StdinputGetInputEvent,
    .DeviceInit = StdinputDeviceInit,
    .DeviceExit = StdinputDeviceExit,
};

void StdinputRegister() {
    RegisterInput(&g_tStdinputDev);
}
