#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define PATH_MAX 1024

int main(int argc,char** argv) {
	char* buf = (char*)malloc(sizeof(char)*PATH_MAX);
	char* cwd=NULL;
	size_t size = PATH_MAX;
	while((cwd=getcwd(buf,size))==NULL) {
		if(errno==ERANGE) {
			size*=2;
			buf = (char*)realloc(buf,size);
		}else {
			perror("getcwd()");
			exit(EXIT_FAILURE);
		}
	}

	puts(buf);
	free(buf);


	return 0;
}
