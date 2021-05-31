#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_ARGC 1024
#define MAX_ARGLEN 1024 

int execute(char* argvs[]) {
    execvp(argvs[1],argvs+1);
    perror("execvp()");
    exit(1);
}

char* makeString(char arg[]) {
    arg[strlen(arg)-1]='\0';//fgets得到的字符串自动追加换行
    char* res = (char*)malloc(strlen(arg)+5);
    strcpy(res,"/bin/");
    return strcat(res,arg);
}

int main()
{
    int countArgc = 1;
    char arg[MAX_ARGLEN];
    char* argvs[MAX_ARGC];
    while(countArgc<MAX_ARGC){
        printf("Input the [%d] argv: ",countArgc);
        if(fgets(arg,MAX_ARGLEN,stdin)!=NULL&&strcmp(arg,"\n")!=0) {
            argvs[countArgc++]=makeString(arg);     
        }else if(countArgc>1){
            argvs[countArgc]=NULL;
            execute(argvs);
            countArgc=1;
        }
        puts(arg);
    }
    return 0;
}

