#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <curses.h>

#define exitErr(func) {perror(func);exit(EXIT_FAILURE);}
#define PATH_MAX 1024

#define LEFTEDGE 10
#define RIGHTEDGE 30
#define ROW     10

int main(int argc,char** argv) {
    initscr();
       
    char* msg = "hello!";
    char* blank = "      ";
    int dir = 1;
    int pos = LEFTEDGE;
    
    clear();
    while(1){
        move(ROW,pos);
        addstr(msg);
        move(LINES-1,COLS-1);
        refresh();
        usleep(60000);
        move(ROW,pos);
        addstr(blank);
        pos+=dir;
        if(pos>=COLS-sizeof(msg)) dir=-1;
        if(pos<=0) dir=1;
    }

    getch();
    endwin();
}
