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
scp yc@10.0.2.2:/home/yc/lab/project/test .
scp yc@10.0.2.2:/home/yc/lab/project/client/client .
./test &
./client 127.0.0.1
```
