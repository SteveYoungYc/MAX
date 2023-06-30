#ifndef _AT24C02_H
#define _AT24C02_H

#define IOC_AT24C02_READ 100
#define IOC_AT24C02_WRITE 101

typedef struct at24c02 {
    char name[32];
    int fd;
    char buf[8];
} at24c02;

extern at24c02 eeprom;

int at24c02_init(at24c02* eeprom);
int at24c02_exit(at24c02* eeprom);
int at24c02_ctl(at24c02* eeprom, int request, char addr, char* data);

#endif
