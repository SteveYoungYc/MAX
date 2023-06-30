#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <key/key.h>

key key0;

int key_init(key* key) {
    char name_buff[32];
    sprintf(name_buff, "/dev/%s", key->name);
    key->fd = open(name_buff, O_RDWR);
    if (key->fd < 0) {
        printf("can't open /dev/%s\n", key->name);
        return -1;
    }
    return 0;
}

int key_exit(key* key) {
    close(key->fd);
    return 0;
}

int key_read(key* key) {
    if (read(key->fd, &key->value, 4) == 4) {
        return 0;
    }
    return -1;
}
