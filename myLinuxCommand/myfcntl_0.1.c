#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define PATH_MAX 1024

int main(int argc,char** argv) {
	if(argc < 2) {
		fprintf(stderr,"Usage...\n");
		exit(1);
	}
	int fd = open(argv[1],O_RDWR|O_TRUNC|O_CREAT);
	if(fd==-1) {
		perror("open()");
		exit(1);
	}
	int s = fcntl(fd,F_GETFL);
	s |= O_SYNC; //关闭缓冲
	s |= O_APPEND; //追加模式
	if(fcntl(fd,F_SETFL,s)==-1) {
		perror("fcntl()");
		exit(1);
	}
	puts("OK");
	close(fd);
	return 0;
}
