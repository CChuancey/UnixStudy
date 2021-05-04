#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define BUFFERSIZE 1024

int main(int argc,char **argv) {
    if(argc < 3) {
        fprintf(stderr,"Usage...\n");
        exit(1);
    }
    char* src = argv[1];
    char* dst = argv[2];
    mode_t pre_umask=umask(0);
    struct stat srcfileStat;
    struct stat dstfileStat;
    if(stat(src,&srcfileStat)==-1||stat(dst,&dstfileStat)==-1) {
        perror("stat()");
        exit(1);
    }
    mode_t mode = srcfileStat.st_mode&07777;
    
    //默认第一个参数是不带有路径的文件名
    if(S_ISDIR(dstfileStat.st_mode)) {
        strcat(dst,"/");
        strcat(dst,src);
    }

    int fd_dst = open(dst,O_CREAT|O_TRUNC|O_WRONLY,mode); // 当file access的参数中有O_CRET时，必须指定文件权限 
    umask(pre_umask); //恢复umask值

    
    int fd_src = open(src,O_RDONLY);
    if(fd_src == -1) {
        perror("src open():");
        exit(1);
    }
    if(fd_dst == -1) {
        perror("dst open():");
        close(fd_src);
        exit(1);
    }
    while(1) {
        char buffer[BUFFERSIZE];
        int count = read(fd_src,buffer,BUFFERSIZE);
        if(count==-1) {
            perror("read");
            exit(1);
        }
        if(count==0) break;

        count = write(fd_dst,buffer,count);
        if(count==-1) {
            perror("write");
            exit(1);
        }
    }

    close(fd_src);
    close(fd_dst);
    return 0;
}
