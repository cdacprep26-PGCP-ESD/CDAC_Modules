#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include<sys/ioctl.h>
#include"ioctl.h"
#include<stdint.h>
int main(void)
{
	int fd;
	char buf[64];
	ssize_t n;
	unsigned long temp = 0;

	fd = open("/dev/Character_Timer_Device", O_RDONLY);
	if (fd < 0) {
		perror("open");
		return 1;
	}

	/* read call 1 */
	n = read(fd, buf, 20);
	buf[n] = '\0';
	printf("%s\n", buf);
	
	ioctl(fd, GET_AVA_SPACE, &temp);
	printf("temp: %d\n", temp);
	ioctl(fd, GET_CNT, &temp);
	printf("temp: %d\n", temp);
	
	temp = 0;
	ioctl(fd, SET_CNT, &temp );
	printf("temp: %d\n", temp);
	
	ioctl(fd, GET_CNT, &temp);
	printf("temp: %d\n", temp);

	return 0;
}
