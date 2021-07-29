#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "socklib.c"
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>

static int sockfd=-1;

#define BUFFSIZE 4096

int setnonblock(int fd){
    int old = fcntl(fd,F_GETFL);
    int new_ = old | O_NONBLOCK;
    fcntl(fd,F_SETFL,new_);
    return old;
}

int main(int argc,char** argv)
{
    sockfd = connect_to_server("127.0.0.1",12346);
    if(sockfd == -1) {
        exitErr("connect_to_server");
    }
    setnonblock(sockfd);
    char* headers = "POST /upload HTTP/1.1\r\n"
        "Content-Type: multipart/form-data; boundary=--------------------------813208548743608915982962\r\n"
        "User-Agent: PostmanRuntime/7.28.2\r\n"
        "Accept: */*\r\n"
        "Postman-Token: 0c3cf63c-7a91-44bf-b48a-a295526da262\r\n"
        "Host: 192.168.136.128:12346\r\n"
        "Accept-Encoding: gzip, deflate, br\r\n"
        "Connection: keep-alive\r\n"
        "Content-Length: 2518238\r\n\r\n"
        ;
    send(sockfd,headers,strlen(headers),0);
    int fd = open("1.jpg",O_RDONLY);
    if(fd==-1) exitErr("open");
    char buf[BUFFSIZE];

    char* boundary = "----------------------------813208548743608915982962\r\n"
        "Content-Disposition: form-data; name=\"\"; filename=\"ferrari-430-scuderia-3840x2160-2018-cars-autumn-road-5k-16855.jpg\"\r\n"
        "Content-Type: image/jpeg\r\n\r\n";
    send(sockfd,boundary,strlen(boundary),0);
    int file_len=0;
    while(1){
        int len = read(fd,buf,BUFFSIZE);
        if(len==-1){
            exitErr("read");
        }
        if(len==0) break;
        int rdx=0;
        while(1){
            int ret = send(sockfd,buf+rdx,len-rdx,0);
            if(ret==0){
                if(rdx>=len) break;
                else {
                    fprintf(stderr,"send error\n");
                    break;
                }
            }else if(ret==-1){
                if(errno==EAGAIN||errno==EWOULDBLOCK){
                    perror("send");
                    break;
                }
                perror("send");
            }
            rdx+=ret;
            printf("ret:%d rdx:%d\n",ret,rdx);

        }
        file_len+=len;
    }
    printf("file len:%d\n",file_len);
    sleep(2);
    return 0;
}

