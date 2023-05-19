#include <input_manager.h>

int main(int argc, char** argv) {
    int ret;
    InputEvent event;
    InputManagerInit();
    int count = 100;
    while (count) {
        ret = GetInputEvent(&event);
        if (ret) {
            printf("GetInputEvent err!\n");
            break;
        } else {
            printf("Type      : %d\n", event.iType);
            printf("str       : %s\n", event.data.net.str);
        }
        count--;
    }
    InputManagerExit();
    return 0;
}
