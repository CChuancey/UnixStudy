#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>

#define exitErr(func) {perror(func);exit(EXIT_FAILURE);}

int main()
{
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd==-1) exitErr("socket()");
    struct sockaddr_in serAddr;
    if(inet_aton("127.0.0.1",&serAddr.sin_addr)==0) {
        fprintf(stderr,"inet_aton() error\n");
        exit(EXIT_FAILURE);
    }
    serAddr.sin_port=htons(65534);
    serAddr.sin_family=AF_INET;
    if(bind(sockfd,(struct sockaddr*)&serAddr,sizeof(serAddr))==-1) exitErr("bind()");
    if(listen(sockfd,555)==-1) exitErr("listen()");
    while(1){
        struct sockaddr_in clientSockAddr;
        socklen_t len;
        int clientSockFd = accept(sockfd,(struct sockaddr*)&clientSockAddr,&len);
        if(clientSockFd==-1) exitErr("accept()");
        puts("connected!");
    }
    return 0;
}

