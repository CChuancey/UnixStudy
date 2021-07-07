#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "socklib.c"
#include <unistd.h>
#include <signal.h>

static int sockfd=-1;

#define BUFFSIZE 1024

int main(int argc,char** argv)
{
    sockfd = connect_to_server("127.0.0.1",12346);
    if(sockfd == -1) {
        exitErr("?");
    }
    char buf[BUFFSIZE];
    while(1){
        fgets(buf,BUFFSIZE-1,stdin);
        buf[strlen(buf)-1]='\0';
        send(sockfd,buf,strlen(buf),0);
        recv(sockfd,buf,strlen(buf),0);
        puts(buf);
    }
    return 0;
}

