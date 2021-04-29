#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <utmp.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>

#define UTMP "/var/run/utmp"

void showMessage(struct utmp* myutmp) {
    if(myutmp->ut_type==USER_PROCESS) {
        printf("%-8.8s %-13.8s",myutmp->ut_user,myutmp->ut_line);
        //       printf("%ld",myutmp->ut_tv.tv_sec);
        //       long time_sec = myutmp->ut_tv.tv_sec; //必须用long 接受，int32_t不行
        time_t time_sec = myutmp->ut_tv.tv_sec; //
        struct tm* actualTime = gmtime(&time_sec);
        //        puts("OK");
        //if(actualTime==NULL) puts("hello");
        printf("%4d-%02d-%02d ",actualTime->tm_year+1900,actualTime->tm_mon+1,actualTime->tm_mday);
        printf("%02d:%02d ",actualTime->tm_hour,actualTime->tm_min);
        if(myutmp->ut_host!=NULL&&strcmp("",myutmp->ut_host)!=0)
            printf("(%s)",myutmp->ut_host);
        //        puts(ctime(&time_sec));
        printf("\n");
    }
}

int main(int argc,char **argv) {
    struct utmp myutmp;
    int fd = open(UTMP,O_RDONLY);
    if(fd==-1) {
        perror("open():");
        exit(1);
    }
    int len = sizeof(myutmp);

    if(argc==3&&strcmp(argv[1],"am")==0&&strcmp(argv[2],"i")==0) {
        char* mytty = ttyname(0)+5;
        if(mytty==NULL) {
            perror("ttyname()");
            exit(1);
        }
        while(read(fd,(void *)&myutmp,len)==len) {
            if(strncmp(myutmp.ut_line,mytty,UT_LINESIZE)==0) 
                showMessage(&myutmp);
        }
    }else{
        while(read(fd,(void *)&myutmp,len)==len) {
            showMessage(&myutmp);
        }
    }


    close(fd);
    return 0;
}
