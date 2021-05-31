#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define LINE_LENGTH 1024
#define exitErr(func) {perror(func);exit(EXIT_FAILURE);}

int main()
{
    char line[LINE_LENGTH];
    fgets(line,100,stdin);
    puts(line);

    int fd = open("/etc/services",O_RDONLY);
    close(0);
    if(fd==-1) exitErr("open()");
    int newfd = dup(fd);
    if(newfd!=0){
        fprintf(stderr,"标准输入正在使用！\n");
        exit(EXIT_FAILURE);
    }
    close(fd);


    fgets(line,100,stdin);
    puts(line);

    return 0;
}

