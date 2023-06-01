#include <disp_manager.h>
#include <font_manager.h>
#include <input_manager.h>
#include <page_manager.h>
#include <util/priority_queue.h>
#include <led/led.h>

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
    for (int i = 0; i < LED_NUM; i++) {
        sprintf(leds[i].name, "100ask_led%d", i);
        printf("led name: %s\n", leds[i].name);
        LEDInit(&leds[i]);
        LEDSetStatus(&leds[i], 0);
    }
}

static void MainPageExit() {
    for (int i = 0; i < LED_NUM; i++) {
        LEDExit(&leds[i]);
    }
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
            case INPUT_TYPE_NET:
                displayString(event.data.net.str, 0, 32, ptDispBuff);
                break;
            case INPUT_TYPE_STD:
                if (strcmp(event.data.std.str, "/quit") == 0) {
                    return;
                }
                if (strcmp(event.data.std.str, "/clear") == 0) {
                    Clear();
                    break;
                }
                if (strcmp(event.data.std.str, "/ledon") == 0) {
                    LEDSetStatus(&leds[0], 1);
                    break;
                }
                if (strcmp(event.data.std.str, "/ledoff") == 0) {
                    LEDSetStatus(&leds[0], 0);
                    break;
                }
                displayString(event.data.std.str, 0, 48, ptDispBuff);
                break;
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
