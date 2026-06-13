#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>


char buf[10];

int main(int argc, char *argv[])
{
	int fd;
	
	if(argc < 2 || argc > 3){
		printf("usage: ./test <dev> [string]\n");
		return 0;
	}

	
	fd = open(argv[1], O_RDWR);
	if(fd < 0){
		printf("file open failed\n");
		return 1;
	}

	if(argc == 2){
		read(fd, buf, 1);
	}
	
	if(argc == 3){
		buf[0] = atoi(argv[2]); //string to int
		write(fd, buf, 1);
	}

	close(fd);
	
	return 0;
}

