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

int make_server_socket(int portnum,int backlog) {
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd==-1) return -1;
    struct sockaddr_in serAddr;
    if(inet_aton("127.0.0.1",&serAddr.sin_addr)==0) {
        fprintf(stderr,"inet_aton() error\n");
        return -1;
    }
    serAddr.sin_port=htons(portnum);
    serAddr.sin_family=AF_INET;
    if(bind(sockfd,(struct sockaddr*)&serAddr,sizeof(serAddr))==-1) return -1;
    if(listen(sockfd,backlog)==-1) return -1;
    return sockfd;
}

/*
 * 返回向服务器通信的fd
 * */
int connect_to_server(char* host,int portnum) {
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd==-1) return -1;
    struct sockaddr_in serSockAddr;
    inet_aton(host,&serSockAddr.sin_addr);
    serSockAddr.sin_family = AF_INET;
    serSockAddr.sin_port = htons(portnum);
    if(connect(sockfd,(struct sockaddr*)&serSockAddr,sizeof(serSockAddr))==0)
        return sockfd;
    return -1;
}


