#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

int main()
{
    pid_t pid = fork();
    assert(pid!=-1);
    if(pid>0) exit(0);
    umask(0); //mod&0777
    pid_t sid = setsid();
    assert(sid!=-1);
    if(chdir("/")==-1) exit(-1);
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    open("/dev/null",O_RDONLY);
    open("/dev/null",O_WRONLY);
    open("/dev/null",O_WRONLY);
    int fd = open("/tmp/log",O_RDWR|O_CREAT,0777);
    while(1){
        sleep(1);
        write(fd,"ok\n",4);
    }

    return 0;
}

