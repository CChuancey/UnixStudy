#include <stdio.h>
#include <stdlib.h>
#include "socklib.c"
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    int sockfd = make_server_socket(12346,10);
    if(sockfd==-1) exitErr("");
    while(1){
        struct sockaddr_in clientSockAddr;
        socklen_t len=sizeof(clientSockAddr); //必须对len进行初始化！！！
        int clientSockfd = accept(sockfd,(struct sockaddr*)&clientSockAddr,&len);
        if(clientSockfd==-1) exitErr("accept()");
        char clientAddr[INET_ADDRSTRLEN];
        pid_t pid = fork();
        switch(pid){
            case 0:
                if(inet_ntop(AF_INET,&clientSockAddr.sin_addr,clientAddr,INET_ADDRSTRLEN)==NULL)
                    exitErr("inet_ntop()");
                printf("%s connected!\n",clientAddr);
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

