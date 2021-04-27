#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <utmp.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <time.h>

#define UTMP "/var/run/utmp"

void showMessage(struct utmp* myutmp) {
    if(myutmp->ut_type==USER_PROCESS) {
        printf("%-8.8s %-16.8s\t\t",myutmp->ut_user,myutmp->ut_line);
 //       printf("%ld",myutmp->ut_tv.tv_sec);
        printf("%12.12s",ctime(&(myutmp->ut_tv.tv_sec)));
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
    while(read(fd,(void *)&myutmp,len)==len) {
        showMessage(&myutmp);
    }

    close(fd);
    return 0;
}
