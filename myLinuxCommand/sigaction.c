#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/time.h>

#define exitErr(func) {perror(func);exit(EXIT_FAILURE);}
#define PATH_MAX 1024
#define INPUTLEN 100

void intHandler(int s){
    printf("\nsignum: %d received!\n",s);
    sleep(2);
    puts("Leaving intHandler!");
}

void quitHandler(int s){
    printf("\nsignum: %d received!\n",s);
    sleep(2);
    puts("Leaving quitHandler!");

}

int main(int argc,char** argv) {
    struct sigaction action;
    sigemptyset(&action.sa_mask);
    sigaddset(&action.sa_mask,SIGQUIT); //阻塞SIGQUIT信号
    action.sa_handler=intHandler;
    action.sa_flags=SA_RESETHAND; //调用处理信号后就被销毁：捕鼠器模式；    
    if(sigaction(SIGINT,&action,NULL)==-1) exitErr("sigaction()");
    char input[PATH_MAX];
    do{
        puts("Type a msg");
        int nchars = read(0,input,(INPUTLEN-1));
        if(nchars==-1){
            exitErr("read()");
        }else {
            input[INPUTLEN]='\0';
            puts(input);
        }
        
    }while(strncmp(input,"quit",4)!=0);
}
