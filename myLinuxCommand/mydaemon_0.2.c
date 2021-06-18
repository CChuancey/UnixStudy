#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>

int main()
{
    if(daemon(0,0)==-1) {
        perror("daemon()");
        exit(-1);
    }
    int fd = open("/tmp/log",O_WRONLY|O_CREAT,0600);
    assert(fd!=-1);
    while(1){
        sleep(1);
        write(fd,"ok\n",4);
    }
    return 0;
}

