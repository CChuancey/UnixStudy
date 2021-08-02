#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int main()
{
    struct stat stat_buf;
    if(stat(".",&stat_buf)==-1){
        perror("stat()");
    }

    mode_t old = umask(0);
    int ret = mkdir("newfolder",stat_buf.st_mode);
    umask(old);
    return 0;
}

