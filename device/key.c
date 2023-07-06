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
    int err;
    struct input_id id;
    key->fd = open("/dev/input/event3", O_RDWR | O_NONBLOCK);
    if (key->fd < 0) {
        printf("can't open /dev/input/event3\n");
        return -1;
    }
	err = ioctl(key->fd, EVIOCGID, &id);
	if (err != 0) {
        return -1;
	}
    printf("bustype = 0x%x\n", id.bustype );
    printf("product = 0x%x\n", id.product );
    printf("version = 0x%x\n", id.version );
    return 0;
}

int key_exit(key* key) {
    close(key->fd);
    return 0;
}

int key_read(key* key) {
    int len;
    len = read(key->fd, &key->event, sizeof(key->event));
    if (len != sizeof(key->event)) {
        return -1;
    }
    printf("get event: type = 0x%x, code = 0x%x, value = 0x%x\n", key->event.type, key->event.code, key->event.value);
    key->code = key->event.code;
    key->value = key->event.value;
    return 0;
}
