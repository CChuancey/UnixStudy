#include <stdio.h>
#include <stdlib.h>
#include "socklib.c"
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    int sockfd = make_server_socket(12346,10);
    if(sockfd==-1) exit(-1);
    while(1){
        struct sockaddr_in clientSockAddr;
        socklen_t len;
        int clientSockfd = accept(sockfd,(struct sockaddr*)&clientSockAddr,&len);
        if(clientSockfd==-1) exitErr("accept()");
        pid_t pid = fork();
        switch(pid){
            case 0:
                puts("one client connected!");
                dup2(clientSockfd,1);
                execlp("/usr/bin/date","date",NULL);
                exitErr("execlp()");     
            case -1:
                return -1;
            default:
                wait(NULL);
                close(clientSockfd);
        }
    }
}

