# Embedded Linux Project
## Config
## Build
In $pwd folder:
```shell
make
```
In $pwd/client folder:
```shell
arm-linux-gnueabihf-gcc -o client client.c
```
## Run
In IMX6UL qemu:
```shell
scp yc@10.0.2.2:/home/yc/lab/project/MAX/test .
scp yc@10.0.2.2:/home/yc/lab/project/MAX/client/client .
scp yc@10.0.2.2:/home/yc/lab/project/driver/sr501/key_drv.ko .
scp yc@10.0.2.2:/home/yc/lab/project/driver/sr501/at24c02_drv.ko .
./test &
./client 127.0.0.1
```
