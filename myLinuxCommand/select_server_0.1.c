#include <stdio.h>
#include <stdlib.h>
#include "socklib.c"
#include <assert.h>
#define max(a,b) a>b?a:b;

#define BUFFSIZE 1024

static int max_fd=0;

int main()
{
    int serSockFd = make_server_socket(12346,500);
    max_fd = serSockFd;
    assert(serSockFd!=-1);
    char buf[BUFFSIZE];
    struct sockaddr clientSock;
    socklen_t len = sizeof(clientSock);
    int clientSockFd=-1;
    fd_set workingset,allset;
    FD_ZERO(&allset);
    FD_SET(serSockFd,&allset);
    while(1){
        memset(buf,'\0',sizeof(buf));
        memcpy(&workingset,&allset,sizeof(allset));
        int nums = select(max_fd+1,&workingset,NULL,NULL,NULL);
        assert(nums!=-1);
        int clientSockFd;
        if(FD_ISSET(serSockFd,&workingset)){ //监听本地socket检测是否有连接请求
            puts("one client request connect!");
            clientSockFd = accept(serSockFd,(struct sockaddr*)&clientSock,&len);
            max_fd = max(max_fd,clientSockFd);
            FD_SET(clientSockFd,&allset);
            puts("client connected successfully");
        }else{ //一定是客户端socket来数据了
            for(int i=0;i<=max_fd;i++){
                if(FD_ISSET(i,&workingset)){
                    recv(i,buf,BUFFSIZE,0);
                    puts(buf);
                    send(i,buf,BUFFSIZE,0);
                }
            }
        }
    }
    return 0;
}

