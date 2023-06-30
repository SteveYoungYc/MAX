#ifndef _KEY_H
#define _KEY_H

#define KEY_NUM 2

typedef struct key {
    char name[32];
    int fd;
    int value;
} key;

extern key key0;

int key_init(key* key);
int key_exit(key* key);
int key_read(key* key);

#endif
