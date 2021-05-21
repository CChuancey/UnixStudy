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
    struct termios info;
    int attr = tcgetattr(0,&info);
    if(attr==-1) exitErr("tcgetattr()"); 
    if(info.c_lflag&ECHO) {
        puts("echo enabled!");
    } else {
        puts("echo disabled!");
    }
}
