#include <stdio.h>
#include <stdlib.h>
#include "socklib.c"
#include <assert.h>
#include <poll.h>
#define max(a,b) a>b?a:b;
#define exitErr(func) {perror(func);exit(EXIT_FAILURE);}

#define BUFFSIZE 1024
#define FDNUMS 1024

int main()
{
    int serSockFd = make_server_socket(12346,500);
    assert(serSockFd!=-1);
    char buf[BUFFSIZE];
    struct sockaddr clientSock;
    socklen_t len = sizeof(clientSock);
    int clientSockFd=-1;

    struct pollfd fds[FDNUMS];
    memset(fds,0,sizeof(fds));
    fds[0].fd=serSockFd;
    fds[0].events=POLLIN;
    int countFds = 1;


    while(1){
        memset(buf,0,sizeof(buf));
        int nums = poll(fds,FDNUMS,-1);
        if(nums<0) exitErr("poll()");
        if(fds[0].revents&POLLIN){
            clientSockFd=accept(serSockFd,(struct sockaddr*)&clientSock,&len);
            puts("one client connected!");
            fds[countFds].fd = clientSockFd;
            fds[countFds].events = POLLIN;
            countFds++;
        }else{
            for(int i=1;i<countFds;i++){
                if(fds[i].revents&POLLIN){
                    recv(fds[i].fd,buf,BUFFSIZE,0);
                    puts(buf);
                    send(fds[i].fd,buf,strlen(buf),0);
                }
            }
        }
    }
    return 0;
}

