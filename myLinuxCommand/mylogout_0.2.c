#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <utmp.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

#define PATH "/var/run/utmp"

int main() {
    struct utmp me;
    int fd = open(PATH,O_RDWR);
    if(fd==-1) { //必须用管理员身份运行
        perror("open()");
        exit(1);
    
    }
    char* mytty=ttyname(0)+5;
    while(read(fd,&me,sizeof(me))==sizeof(me)) {
        if(me.ut_type==USER_PROCESS&&strncmp(mytty,me.ut_line,UT_LINESIZE)==0) { //这里要用strncmp！否则报nonstring
            lseek(fd,-sizeof(me),SEEK_CUR);
            me.ut_type=DEAD_PROCESS;
            struct timeval tv;
            gettimeofday(&tv,NULL);
            me.ut_tv.tv_sec = tv.tv_sec;
            me.ut_tv.tv_usec = tv.tv_usec;
            if(write(fd,&me,sizeof(me))==-1) {
                perror("write()");
                exit(1);
            }
        }
    }

    close(fd);
    return 0;
}
