#include <disp_manager.h>
#include <font_manager.h>
#include <input_manager.h>
#include <led/led.h>
#include <eeprom/at24c02.h>
#include <page_manager.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/priority_queue.h>

#define MAX_COMMAND_LENGTH 32
#define MAX_ARGS 10

int displayString(char* str, int lcd_x, int lcd_y, PDispBuff ptBuffer) {
    int i = 0;
    int error;
    FontBitMap tFontBitMap;
    while (str[i] || i * 8 < ptBuffer->iXres) {
        /* get bitmap */
        char c = str[i] ? str[i] : ' ';
        tFontBitMap.iCurOriginX = lcd_x;
        tFontBitMap.iCurOriginY = lcd_y;
        error = GetFontBitMap(c, &tFontBitMap);
        if (error) {
            printf("SelectAndInitFont err\n");
            return -1;
        }

        /* draw on buffer */
        DrawFontBitMap(&tFontBitMap, 0xff0000);

        /* flush to lcd/web */
        FlushDisplayRegion(&tFontBitMap.tRegion, ptBuffer);

        lcd_x = tFontBitMap.iNextOriginX;
        lcd_y = tFontBitMap.iNextOriginY;
        i++;
    }
    return 0;
}

static void MainPageInit() {
    // for (int i = 0; i < LED_NUM; i++) {
    //     sprintf(leds[i].name, "100ask_led%d", i);
    //     printf("led name: %s\n", leds[i].name);
    //     LEDInit(&leds[i]);
    //     LEDSetStatus(&leds[i], 0);
    // }
    strcpy(eeprom.name, "myat24c02");
    at24c02_init(&eeprom);
}

static void MainPageExit() {
    // for (int i = 0; i < LED_NUM; i++) {
    //     LEDExit(&leds[i]);
    // }
    Clear();
}

static void MainPageRun(void* pParams) {
    int ret;
    char output[64];
    InputEvent event;
    PDispBuff ptDispBuff = GetDisplayBuffer();

    displayString("MAX                                                            ", 0, 0, ptDispBuff);
    while (1) {
        ret = GetInputEvent(&event);
        if (ret) {
            break;
        } else {
            int buff_size = getBuffSize(&gRingBuffer);
            snprintf(output, sizeof(output), "buff size: %-64d", buff_size);
            displayString(output, 0, 80, ptDispBuff);
            switch (event.iType) {
                case INPUT_TYPE_TOUCH:
                    break;
                case INPUT_TYPE_NET: {
                    // displayString(event.data.net.str, 0, 32, ptDispBuff);
                    for (int i = 0; i < 8; i++) {
                        event.data.net.str[i]--;
                    }
                    float x = *(float*)event.data.net.str;
                    float y = *(float*)(event.data.net.str + sizeof(float));
                    // printf("[main page] x: %f, y: %f\n", x, y);
                    PutPixel((int)x, (int)y, 0xff00ff);
                    break;
                }
                case INPUT_TYPE_STD: {
                    char* input = event.data.std.str;
                    if (input[0] != '/') {
                        displayString(input, 0, 48, ptDispBuff);
                        break;
                    }

                    char command[MAX_COMMAND_LENGTH];
                    char args[MAX_ARGS][MAX_COMMAND_LENGTH];
                    char* token = strtok(input, " ");
                    strncpy(command, token + 1, sizeof(command) - 1);
                    command[sizeof(command) - 1] = '\0';
                    int argCount = 0;
                    while (token != NULL && argCount < MAX_ARGS) {
                        token = strtok(NULL, " ");
                        if (token != NULL) {
                            strncpy(args[argCount], token, sizeof(args[argCount]) - 1);
                            args[argCount][sizeof(args[argCount]) - 1] = '\0';
                            argCount++;
                        }
                    }

                    if (strcmp(command, "quit") == 0) {
                        return;
                    }
                    if (strcmp(command, "clear") == 0) {
                        Clear();
                        break;
                    }
                    if (strcmp(command, "led") == 0) {
                        if (argCount != 2) {
                            printf("Usage: /led <led index> <status(0/1)>\n");
                            break;
                        }
                        int led_idx = atoi(args[0]);
                        if (led_idx < 0 || led_idx >= LED_NUM) {
                            printf("LED index error!\r\n");
                            break;
                        }
                        int status = atoi(args[1]);
                        if (status != 0 && status != 1) {
                            printf("LED status error!\r\n");
                            break;
                        }
                        LEDSetStatus(&leds[led_idx], status);
                        break;
                    }
                    if (strcmp(command, "at24c02") == 0) {
                        if (argCount != 3) {
                            printf("Usage: /led <r/w> <address> <data>\n");
                            break;
                        }
                        char address;
                        char data;
                        if (*args[0] == 'r') {
                            address = atoi(args[1]);
                            printf("[read] addr: %d\n", address);
                            at24c02_ctl(&eeprom, IOC_AT24C02_READ, address, eeprom.buf);
                        } else if (*args[0] == 'w') {
                            address = atoi(args[1]);
                            data = atoi(args[2]);
                            printf("[write] addr: %d, data: %d\n", address, data);
                            at24c02_ctl(&eeprom, IOC_AT24C02_WRITE, address, &data);
                        }
                        break;
                    }
                    break;
                }
                case INPUT_TYPE_KEY: {
                    printf("[key] value: %x\n", event.data.key.value);
                    break;
                }
                default:
                    break;
            }
        }
    }
}

static PageAction g_tMainPage = {
    .name = "main",
    .Init = MainPageInit,
    .Exit = MainPageExit,
    .Run = MainPageRun,
};

void MainPageRegister(void) {
    PageRegister(&g_tMainPage);
}
