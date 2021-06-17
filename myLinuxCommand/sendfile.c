#include <stdio.h>
#include <stdlib.h>
#include "socklib.c"
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>

#define FILE_PATH "/etc/services"

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
                int fd = open(FILE_PATH,O_RDONLY);
                if(fd==-1) exitErr("open()");
                struct stat statbuf;
                if(fstat(fd,&statbuf)==-1) exitErr("fstat()");
                ssize_t len = 0;
                while(len<statbuf.st_size){
                     if(sendfile(clientSockfd,fd,&len,statbuf.st_size)==-1) exitErr("sendfile()");
                }
            case -1:
                return -1;
            default:
                wait(NULL);
                close(clientSockfd);
        }
    }
}

