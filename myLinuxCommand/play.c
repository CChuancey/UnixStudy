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
    puts("Game Begin!");
    struct termios info;
    int attr = tcgetattr(0,&info);
    if(attr==-1){
        exitErr("tcgetattr()");
    }
    info.c_lflag &= ~ICANON;
    info.c_lflag &= ~ECHO;
    info.c_cc[VMIN] = 1; // 非规范化情况，需要设置每次最少读一个 
    if(tcsetattr(0,TCSANOW,&info)==-1) exitErr("tcsetattr()");
    int ch;
    while(ch=getchar()) {
        switch(ch){
            case 'y':
                puts("Game continue!");
                break;
            case 'n':
                puts("Game Over!");
                exit(EXIT_SUCCESS);
                break;
            default:
                puts("Instruction error!");
        }
    }
}
