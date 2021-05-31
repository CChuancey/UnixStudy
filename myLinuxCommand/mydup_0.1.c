#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define LINE_LENGTH 1024
#define exitErr(func) {perror(func);exit(EXIT_FAILURE);}

int main()
{
    char line[LINE_LENGTH];
    fgets(line,100,stdin);
    puts(line);

    close(0);
    if(open("/etc/services",O_RDONLY)!=0)
        exitErr("open()");

    fgets(line,100,stdin);
    puts(line);

    return 0;
}

