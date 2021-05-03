#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>

int main(int argc,char ** argv) {
    DIR* dir = NULL;
    if(argc==1) dir=opendir(".");
    else dir = opendir(argv[1]);
    if(dir==NULL) {
        perror("opendir()");
        exit(1);
    }

    struct dirent* item;
    errno = 0;
    while((item=readdir(dir))!=NULL) {
        printf("%s\n",item->d_name);
    }
    if(errno!=0) {
        perror("readdir()");
        exit(1);
    }

    closedir(dir);

    return 0;
}
