#include <disp_manager.h>
#include <font_manager.h>
#include <input_manager.h>
#include <page_manager.h>

int displayString(char* str, int lcd_x, int lcd_y, PDispBuff ptBuffer) {
    int i = 0;
    int error;
    FontBitMap tFontBitMap;
    while (str[i]) {
        /* get bitmap */
        tFontBitMap.iCurOriginX = lcd_x;
        tFontBitMap.iCurOriginY = lcd_y;
        error = GetFontBitMap(str[i], &tFontBitMap);
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
    displayString("MAX", 0, 0, ptDispBuff);
    while (1) {
        ret = GetInputEvent(&event);
        if (ret) {
            printf("GetInputEvent err!\n");
            break;
        } else {
            printf("Type      : %d\n", event.iType);
            printf("str       : %s\n", event.data.net.str);
        }
    }
}

static PageAction g_tMainPage = {
    .name = "main",
    .Run = MainPageRun,
};

void MainPageRegister(void) {
    PageRegister(&g_tMainPage);
}
