#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#define PROCESSTREE 0X01
#define MEMSTAT 0X03

int main()
{
	int fd;
	char buff[1024*8] = "4411";
	int ret;

	fd = open("/dev/char_ps",O_RDWR);
	if (fd == -1)
	{
		printf("open error.\n");
		exit(1);
	}
	printf("fd:%d\n",fd);

	ret = ioctl(fd, MEMSTAT, buff);
	printf("ret:%d\n", ret);

	printf("buff:%s\n", buff);

	close(fd);
	return 0;
}
