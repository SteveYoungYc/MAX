#include <input_manager.h>

static int TouchscreenGetInputEvent(PInputEvent ptInputEvent) {
    ptInputEvent->iType = INPUT_TYPE_TOUCH;
    ptInputEvent->data.ts.iX = 0;
    ptInputEvent->data.ts.iY = 0;
    ptInputEvent->data.ts.iPressure = 0;
    gettimeofday(&ptInputEvent->tTime, NULL);

    return 0;
}

static int TouchscreenDeviceInit(void) {
    return 0;
}

static int TouchscreenDeviceExit(void) {
    return 0;
}

InputOpr g_tTouchscreenDev = {
    .name = "touchscreen",
    .GetInputEvent = TouchscreenGetInputEvent,
    .DeviceInit = TouchscreenDeviceInit,
    .DeviceExit = TouchscreenDeviceExit,
};

void TouchscreenRegister() {
    RegisterInput(&g_tTouchscreenDev);
}
