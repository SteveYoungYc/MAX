#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <eeprom/at24c02.h>

at24c02 eeprom;

int at24c02_init(at24c02* eeprom) {
    char name_buff[32];
    sprintf(name_buff, "/dev/%s", eeprom->name);
    eeprom->fd = open(name_buff, O_RDWR);
    if (eeprom->fd < 0) {
        printf("can't open /dev/%s\n", eeprom->name);
        return -1;
    }
    return 0;
}

int at24c02_exit(at24c02* eeprom) {
    close(eeprom->fd);
    return 0;
}

int at24c02_ctl(at24c02* eeprom, int request, char addr, char* data) {
    int buf[2];
    buf[0] = addr;
    switch (request) {
        case IOC_AT24C02_READ: {
            ioctl(eeprom->fd, IOC_AT24C02_READ, buf);
            *data = buf[1];
            break;
        }
        case IOC_AT24C02_WRITE: {
            printf("data: %d\n", *data);
            buf[1] = *data;
            ioctl(eeprom->fd, IOC_AT24C02_WRITE, buf);
            break;
        }
    }
    return 0;
}
