#include <fcntl.h>
#include <linux/fb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <disp_manager.h>
#include <font_manager.h>

int main(int argc, char** argv) {
    PDispBuff ptBuffer;
    int error;

    FontBitMap tFontBitMap;
    char* str = "www.100ask.net";
    int i = 0;
    int lcd_x = 100;
    int lcd_y = 100;

    DisplayInit();

    SelectDefaultDisplay("fb");

    InitDefaultDisplay();

    ptBuffer = GetDisplayBuffer();

    FontsRegister();

    error = SelectAndInitFont("default_font", NULL);
    if (error) {
        printf("SelectAndInitFont err\n");
        return -1;
    }

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
