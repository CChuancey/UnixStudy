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
    struct sockaddr_in serSockAddr;
    inet_aton("127.0.0.1",&serSockAddr.sin_addr);
    serSockAddr.sin_family=AF_INET;
    serSockAddr.sin_port=htons(65534);
    int serSockAddrfd = connect(sockfd,(struct sockaddr*)&serSockAddr,sizeof(serSockAddr));
    if(serSockAddrfd==-1) exitErr("connect()");
    puts("connected@");
    return 0;
}

