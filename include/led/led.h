#ifndef _LED_H
#define _LED_H

#define LED_NUM 4

typedef struct led {
    char name[32];
    int fd;
    int status;
} led;

extern led leds[LED_NUM];

int LEDInit(led* led);
int LEDExit(led* led);
int LEDSetStatus(led* led, int status);

#endif
