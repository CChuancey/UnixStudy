#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define PATH_MAX 1024

int main(int argc,char** argv) {
	char buf[PATH_MAX];
	if(getwd(buf)==NULL) {
		perror("getwd()");
		exit(EXIT_FAILURE);
	}
	puts(buf);


	return 0;
}
