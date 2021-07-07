#include <stdio.h>
#include <stdlib.h>
#include "socklib.c"
#include <assert.h>
#include <sys/epoll.h>
#define max(a,b) a>b?a:b;
#define exitErr(func) {perror(func);exit(EXIT_FAILURE);}

#define BUFFSIZE 1024
#define MAXEVENTS 1024

int main()
{
    int serSockFd = make_server_socket(12346,500);
    assert(serSockFd!=-1);
    char buf[BUFFSIZE];
    struct sockaddr clientSock;
    socklen_t len = sizeof(clientSock);
    int clientSockFd=-1;
    
    int epfd = epoll_create(1);
    assert(epfd!=-1);
    struct epoll_event event;//要监听的事件
    event.events = EPOLLIN;
    event.data.fd = serSockFd;
    if(epoll_ctl(epfd,EPOLL_CTL_ADD,serSockFd,&event)==-1) exitErr("epoll_ctl()");
    while(1){
        memset(buf,0,sizeof(buf));
        struct epoll_event ready_events[MAXEVENTS];//准备好的事件列表
        int nums = epoll_wait(epfd,ready_events,1,-1);
        assert(nums!=-1);
        for(int i=0;i<nums;i++){
            int fd = ready_events[i].data.fd;
            if(fd==serSockFd){
                clientSockFd = accept(serSockFd,(struct sockaddr*)&clientSock,&len);
                puts("one client connected!");
                event.data.fd = clientSockFd;
                event.events = EPOLLIN | EPOLLET; //可读和边缘触发
                epoll_ctl(epfd,EPOLL_CTL_ADD,clientSockFd,&event);
            } else {
                recv(fd,buf,BUFFSIZE,0);
                puts(buf);
                send(fd,buf,strlen(buf),0);
            }
        }
    }

    return 0;
}

