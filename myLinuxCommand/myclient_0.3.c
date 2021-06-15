#include <stdio.h>
#include <stdlib.h>
#include "socklib.c"
#include <unistd.h>

int main()
{
    int sockfd = connect_to_server("127.0.0.1",12346);
    if(sockfd == -1) {
        exitErr("?");
    }
    
    while(1){
        char ch;
        int c = read(sockfd,&ch,1);
        if(c==-1) exitErr("read()");
        if(c==0) break;
        putchar(ch);
    }
    close(sockfd);
    return 0;
}

