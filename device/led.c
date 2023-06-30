#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <led/led.h>

led leds[LED_NUM];

int LEDInit(led* led) {
    char name_buff[32];
    sprintf(name_buff, "/dev/%s", led->name);
    led->fd = open(name_buff, O_RDWR);
    if (led->fd < 0) {
        printf("can't open /dev/%s\n", led->name);
        return -1;
    }
    return 0;
}

int LEDExit(led* led) {
    close(led->fd);
    return 0;
}

int LEDSetStatus(led* led, int status) {
    int ret;
    ret = write(led->fd, &status, 1);
    if (ret < 0) {
        return -1;
    }
    led->status = status;
    return 0;
}
