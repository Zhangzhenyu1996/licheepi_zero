#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<string.h>
#include<asm-generic/ioctl.h>

#define DEVICE          "/dev/myled"
#define MAGIC_NUM       'k'
#define LED_ON   _IO(MAGIC_NUM, 0)
#define LED_OFF  _IO(MAGIC_NUM, 1)

int main(int argc, char *argv[])
{
       int fd;

        printf("Start led test.\n");

        fd = open(DEVICE, O_RDWR);//O_RDWR：可读可写
        if (fd < 2) {
                printf("Cannot open device %s\n", DEVICE);
                return -EFAULT;
        }       
        while (1) {
                ioctl(fd, LED_ON, 0);//ioctl(int fd,unsigned long cmd,...),fd:文件描述符,cmd 控制命令
                sleep(1);
                ioctl(fd, LED_OFF, 0);
                sleep(1);
        }
        return 0;       
}
