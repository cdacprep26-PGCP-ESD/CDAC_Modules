#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include<sys/ioctl.h>
#include<stdint.h>

#define BEGINNER_GET_LENGTH _IOR('B',1,uint32_t)

int main()
{
    int fd;
    char buf[64];
    ssize_t n;
    
    uint32_t length;

    fd = open("/dev/beginner", O_RDWR);
    if (fd < 0)
    {
        perror("open");
        return 1;
    }
    
    
    if(ioctl(fd, BEGINNER_GET_LENGTH, &length) < 0)
    {
    	perror("Error");
    	return 1;
    }
    
    printf("Length = %u\n",length);
    
    
    strcpy(buf, "Hello from user");

    n = write(fd, buf, strlen(buf));
    if (n < 0)
    {
        perror("write");
        close(fd);
        return 1;
    }

    // lseek(fd, 0, SEEK_SET);

    strcpy(buf, " Hello Kernel\n");

    n = write(fd, buf, strlen(buf));

    if (n < 0)
    {
        perror("write");
        close(fd);
        return 1;
    }

    lseek(fd, 0, SEEK_SET);

    n = read(fd, buf, sizeof(buf) - 1);

    if (n < 0)
    {
        perror("read");
        close(fd);
        return 1;
    }

    buf[n] = '\0';
    printf("Read %zd bytes: %s", n, buf);
    
    
    

    close(fd);
    return 0;
}
