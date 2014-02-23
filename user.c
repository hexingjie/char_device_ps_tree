#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#define PROCESSTREE 0x01
#define THREADGROUP 0x02
#define MEMSTAT 0x03

int main(int argc, char *argv[])
{
	int fd;
	char buff[1024*8] = "2582";
	int ret;

	fd = open("/dev/char_ps",O_RDWR);
	if (fd == -1)
	{
		printf("open error.\n");
		exit(1);
	}
	printf("fd:%d\n",fd);

	if (argc == 2)
	{
		printf("argc:%d,argv[1]:%s.\n",argc, argv[1]);
		if(*argv[1] == 0x01)
		     ret = ioctl(fd, PROCESSTREE, buff);
		else if (*argv[1] == 0x02)
		     ret = ioctl(fd, THREADGROUP, buff);

		printf("ret:%d\n", ret);
		printf("buff:%s\n", buff);
		
	}
	else if (argc == 3)
	{
		strcpy(buff, argv[2]);
		printf("argc:%d,argv[1]:%s. argv[2]:%s\n",argc, argv[1], argv[2]);
		ret = ioctl(fd, MEMSTAT, buff);
		printf("ret:%d\n", ret);
		printf("buff:%s\n", buff);
		
	}
#if 0
	ret = ioctl(fd, MEMSTAT, buff);
	printf("ret:%d\n", ret);

	printf("buff:%s\n", buff);

#endif
	close(fd);
	return 0;
}
