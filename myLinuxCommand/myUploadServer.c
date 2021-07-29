#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <assert.h>
#include "socklib.c"
#include <fcntl.h>
#include <errno.h>


#define BUFF_SIZE 1024
#define exitErr(func) {perror(func);exit(EXIT_FAILURE);}


int setnonblocking(int fd){
    int old_opt = fcntl(fd,F_GETFL);
    int new_opt = old_opt | O_NONBLOCK;
    fcntl(fd,F_SETFL,new_opt);
    return old_opt;
}

void addfd(int epollfd,int fd,int oneshoot){
    struct epoll_event event;
    event.events = EPOLLIN | EPOLLHUP | EPOLLET;
    event.data.fd = fd;
    if(oneshoot) event.events |= EPOLLONESHOT;
    if(epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&event)==-1) exitErr("epoll_ctl");
    setnonblocking(fd);
}

void reset_oneshoot(int epfd,int fd){
    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
    epoll_ctl(epfd,EPOLL_CTL_MOD,fd,&event);
}

int get_line_from_socket(int sockfd,char* buf){
    int i=0;
    char c='\0';
    while(i<BUFF_SIZE-1&&c!='\n'){
        int n = recv(sockfd,&c,1,0);
        if(n>0){
            if(c=='\r'){//将消息的\r\n读进缓冲区为\n
                n=recv(sockfd,&c,1,MSG_PEEK);
                if(n>0&&c=='\n')
                    recv(sockfd,&c,1,0);
                else
                    c='\n';
            }
            buf[i++]=c;
        }else{//包括读错
            c='\n';//读完退出
        }
    }
    buf[i]='\0';
    return i;
}

int main(int argc,char** argv){
    int sockfd = make_server_socket(12346,10);
    if(sockfd==-1) exitErr("make_server_socket");


    struct epoll_event events[1024];
    int epollfd = epoll_create(1);
    assert(epollfd!=-1);

    addfd(epollfd,sockfd,0);

    char* body = "<html><body>end</body></html>";
    int len = strlen(body);
    char* ok_msg = "HTTP/1.1 200 OK\r\nConnection: keep-alive\r\ncontent-length: ";
    char res[200];
    sprintf(res,"%s%d\r\n\r\n%s",ok_msg,len,body);

    char buf[BUFF_SIZE];
    memset(buf,'\0',BUFF_SIZE);
    while(1){
        int ret = epoll_wait(epollfd,events,1024,-1);
        if(ret<0){
            fprintf(stderr,"epoll wait failed!\n");
            break;
        }

        for(int i=0;i<ret;i++){
            int fd = events[i].data.fd;
            if(fd==sockfd){//establish connection
                struct sockaddr_in clientSockAddr;
                socklen_t len = sizeof(clientSockAddr);
                int clientSockfd = accept(sockfd,(struct sockaddr*)&clientSockAddr,&len);
                if(clientSockfd==-1){ //非阻塞
                    if(errno==EAGAIN||errno==EINTR){
                        reset_oneshoot(epollfd,fd);
                        break;
                        // continue;
                    }
                    exitErr("accept");
                }
                puts("one client has established connection!");
                addfd(epollfd,clientSockfd,1);
            }else if(events[i].events&EPOLLIN){
                int idx=0;
                int content_length=0;
                while(1){
                    int bytes_read = get_line_from_socket(fd,buf);
                    if(bytes_read==1&&buf[0]=='\n') break;
                    idx+=bytes_read;
                    if(strncasecmp(buf,"Content-Length: ",15)==0){
                        sscanf(buf+15,"%d",&content_length);
                    }
                    //printf("bytes:%d read,total:%d\n%s",bytes_read,idx,buf);
                    printf("%s",buf);
                }
                send(fd,res,strlen(res),0);
                while(get_line_from_socket(fd,buf)&&strcmp(buf,"\n")!=0)
                    puts(buf);
                
                
                int file = open("1.jpg",O_RDWR|O_TRUNC|O_CREAT,0600);
                if(file==-1) exitErr("open()");
                int file_len = 0;
                while(1){
                    //usleep(1);
                    int len = recv(fd,buf,BUFF_SIZE,0);
                    if(len==0) {
                        close(fd);
                        break;
                    }
                    if(len<0){
                        if(errno==EAGAIN||errno==EWOULDBLOCK||errno==EINTR){
                            break;
                        }
                        exitErr("read");
                    }
                    int write_num = 0;
                    while(1){
                        int ret = write(file,buf+write_num,len-write_num);
                        if(ret==0)
                            break;
                        if(ret==-1){
                            if(errno==EAGAIN||errno==EWOULDBLOCK) {
                                perror("write");
                                break;
                            }
                            exitErr("write");
                        }
                        write_num+=ret;
                    }
                    file_len+=len;
                }

                printf("prcess over,file len:%d\n",file_len);

            }
        }

    }
    close(sockfd);

    return 0;
}

