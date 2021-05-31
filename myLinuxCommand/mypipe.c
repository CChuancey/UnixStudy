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
    int pipefd[2];
    if(pipe(pipefd)==-1){
        exitErr("pipe()");
    }
    pid_t pid = fork();
    if(pid==-1){
        exitErr("fork()");
    } else if(pid==0){//child
        char* msg = "hello";
        if(write(pipefd[1],msg,strlen(msg))!=strlen(msg)){
            exitErr("write()");
        }
        puts("pipe writed!");
        sleep(1);
    } else {//parent
        // close(pipefd[0]);
        sleep(2);
        char buffer[BUFFR_SIZE];
        puts("parent is waiting read form pipe!");
        if(read(pipefd[0],buffer,BUFFR_SIZE)==-1){
            exitErr("read()");
        }
        puts(buffer);
    }
    return 0;
}

