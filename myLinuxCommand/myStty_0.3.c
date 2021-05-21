#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>

#define exitErr(func) {perror(func);exit(EXIT_FAILURE);}
#define PATH_MAX 1024

int main(int argc,char** argv) {
    if(argc==1) {
        fprintf(stderr,"Usage...\n");
        exit(EXIT_FAILURE);
    }
    struct termios info;
    int attr = tcgetattr(0,&info);
    if(attr==-1) exitErr("tcgetattr()"); 
    if(argv[1][0]=='y') {
        info.c_lflag |= ECHO;
    } else {
        info.c_lflag &= ~ECHO;
    }
    if(tcsetattr(0,TCSANOW,&info)==-1){
        exitErr("tcsetattr()");
    }
}
