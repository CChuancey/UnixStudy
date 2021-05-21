#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>

#define PATH_MAX 1024

int main(int argc,char** argv) {
    struct termios info;
    int attr = tcgetattr(0,&info);
    if(attr==-1) {
        perror("tcgetattr()");
        exit(EXIT_FAILURE);
     }
    if(info.c_lflag&ECHO) {
        puts("echo enabled!");
    } else {
        puts("echo disabled!");
    }
}
