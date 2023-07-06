#ifndef _KEY_H
#define _KEY_H

#include <linux/input.h>

#define KEY_NUM 2

typedef struct key {
    int fd;
    int code;
    int value;
    struct input_event event;
} key;

extern key key0;

int key_init(key* key);
int key_exit(key* key);
int key_read(key* key);

#endif
