#include <input_manager.h>
#include <key/key.h>
#include <string.h>

static int KeyGetInputEvent(PInputEvent ptInputEvent) {
    ptInputEvent->iType = INPUT_TYPE_KEY;
    gettimeofday(&ptInputEvent->tTime, NULL);
    if (!key_read(&key0)) {
        ptInputEvent->data.key.id = 0;
        ptInputEvent->data.key.value = key0.value;
        return 0;
    }
    return -1;
}

static int KeyDeviceInit(void) {
    strcpy(key0.name, "100ask_gpio_key");
    key_init(&key0);
    printf("key name: %s\n", "100ask_gpio_key");
    return 0;
}

static int KeyDeviceExit(void) {
    key_exit(&key0);
    return 0;
}

InputOpr g_tKeyDev = {
    .name = "key",
    .GetInputEvent = KeyGetInputEvent,
    .DeviceInit = KeyDeviceInit,
    .DeviceExit = KeyDeviceExit,
};

void KeyinputRegister() {
    RegisterInput(&g_tKeyDev);
}
