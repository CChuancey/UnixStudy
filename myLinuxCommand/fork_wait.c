#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#define exitErr(func) {perror(func); exit(EXIT_FAILURE);}

void childFunc(){
    printf("child process is running,pid is %d\n",getpid());
}

void parentFunc(){
    puts("parent process is waitting child process exit");
    int exitStat;
    pid_t cpid = wait(&exitStat); //exitStat的16位由三部分组成：返回值+发生错误的标志+信号的序号
    int exit_value = exitStat >> 8;
    int core_dump_flag = exitStat & 0x80;
    int signumber = exitStat & 0x7f;
    printf("child process pid:%d has exited"
           ",the exit value is %d"
           ",the exit signumber is %d"
           ",the exit core_dump_flag is %d\n",cpid,exit_value,signumber,core_dump_flag);
}

int main() {
    puts("bg!");
    pid_t cpid = fork();
    if(cpid==-1) {
        exitErr("fork()");
    } else if(cpid==0){ //child
        childFunc();
        exit(EXIT_SUCCESS);
    }else { //parent
        parentFunc();    
    }
    return 0;
}

