#include <disp_manager.h>
#include <font_manager.h>
#include <input_manager.h>
#include <page_manager.h>
#include <util/priority_queue.h>

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

static void MainPageRun(void* pParams) {
    int ret;
    InputEvent event;
    PDispBuff ptDispBuff = GetDisplayBuffer();
    PriorityQueue* pq = pqInit(4);

    displayString("MAX                                                            ", 0, 0, ptDispBuff);
    while (1) {
        ret = GetInputEvent(&event);
        if (ret) {
            break;
        } else {
            switch (event.iType) {
            case INPUT_TYPE_TOUCH:
                break;
            case INPUT_TYPE_NET:
                displayString(event.data.net.str, 0, 32, ptDispBuff);
                break;
            case INPUT_TYPE_STD:
                if (strcmp(event.data.std.str, "/quit") == 0) {
                    goto quit;
                }
                if (strcmp(event.data.std.str, "/clear") == 0) {
                    Clear();
                    break;
                }
                displayString(event.data.std.str, 0, 48, ptDispBuff);
                break;
            default:
                break;
            }
        }
    }
quit:
    Clear();
    pqDestory(pq);
}

static PageAction g_tMainPage = {
    .name = "main",
    .Run = MainPageRun,
};

void MainPageRegister(void) {
    PageRegister(&g_tMainPage);
}
