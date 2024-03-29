#include <fcntl.h>
#include <linux/fb.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <disp_manager.h>

static int fd_fb;
static struct fb_var_screeninfo var; /* Current var */
static int screen_size;
static unsigned char* fb_base;
static unsigned int line_width;
static unsigned int pixel_width;

static int FbDeviceInit(void) {
    // fb1 is my own lcd driver file, fb0 is the original one
    fd_fb = open("/dev/fb1", O_RDWR);
    if (fd_fb < 0) {
        printf("can't open /dev/fb1\n");
        return -1;
    }
    if (ioctl(fd_fb, FBIOGET_VSCREENINFO, &var)) {
        printf("can't get var\n");
        return -1;
    }

    line_width = var.xres * var.bits_per_pixel / 8;
    pixel_width = var.bits_per_pixel / 8;
    screen_size = var.xres * var.yres * var.bits_per_pixel / 8;
    fb_base = (unsigned char*)mmap(NULL, screen_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_fb, 0);
    if (fb_base == (unsigned char*)-1) {
        printf("can't mmap\n");
        return -1;
    }

    return 0;
}

static int FbDeviceExit(void) {
    munmap(fb_base, screen_size);
    close(fd_fb);
    return 0;
}

static int FbGetBuffer(PDispBuff ptDispBuff) {
    ptDispBuff->iXres = var.xres;
    ptDispBuff->iYres = var.yres;
    ptDispBuff->iBpp = var.bits_per_pixel;
    ptDispBuff->buff = (char*)fb_base;
    return 0;
}

static int FbFlushRegion(PRegion ptRegion, PDispBuff ptDispBuff) {
    return 0;
}

static DispOpr g_tFramebufferOpr = {
    .name = "fb",
    .DeviceInit = FbDeviceInit,
    .DeviceExit = FbDeviceExit,
    .GetBuffer = FbGetBuffer,
    .FlushRegion = FbFlushRegion,
};

void FramebufferInit(void) {
    RegisterDisplay(&g_tFramebufferOpr);
}
