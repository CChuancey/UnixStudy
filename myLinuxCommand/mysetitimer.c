#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/time.h>

#define exitErr(func) {perror(func);exit(EXIT_FAILURE);}
#define PATH_MAX 1024

void countDown(int s) {
    static int countNum=10;
    printf("The time: %d \n",countNum--);
    if(countNum==0) {
        puts("OK!");
        exit(EXIT_SUCCESS);
    }
}

int setticker(int interval) {
    struct itimerval value;
    struct timeval val;
    val.tv_sec = interval;
    val.tv_usec = (interval %  1000)*1000;
    value.it_interval = val;
    value.it_value = val;
    return setitimer(ITIMER_REAL,&value,NULL);

}

int main(int argc,char** argv) {
    signal(SIGALRM,countDown);
    if(setticker(1)){
        perror("setitimer()");
        exit(EXIT_FAILURE);
    }
    while(1) pause();

}
