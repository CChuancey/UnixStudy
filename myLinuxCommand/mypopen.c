#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFFR_SIZE 1024
#define exitErr(func) {perror(func);exit(EXIT_FAILURE);}


int main(int argc,char** argv)
{
    int i=0;
    char buf[BUFFR_SIZE];
    FILE* fp = popen("who|sort","r");
    while(fgets(buf,100,fp)!=NULL)
        printf("%3d %s",i++,buf);
    pclose(fp); //pclose中调用了wait可以进行收拾操作，防止了zombie进程的出现
    return 0;
}

