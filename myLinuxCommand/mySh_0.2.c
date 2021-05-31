#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_ARGC 1024
#define MAX_ARGLEN 1024 
#define exitErr(func) {perror(func);exit(EXIT_FAILURE);}

int execute(char* argvs[]) {
    pid_t pid=fork();
    if(pid==0){
        execvp(argvs[1],argvs+1);
        exitErr("fork()");
    }else if(pid==-1){ //考虑fork失败的情况
        exitErr("fork()");
    }else{
        wait(NULL); //先让子进程结束，以免下一条提示符位置错乱
        for(int i=0;argvs[i]!=NULL;i++) free(argvs[i]);//malloc的内存free掉
    }
}

char* makeString(char arg[]) {
    arg[strlen(arg)-1]='\0';//fgets得到的字符串自动追加换行
    char* res = (char*)malloc(strlen(arg)+5);
    if(res==NULL) {
        fprintf(stderr,"no enough memory to apply!\n");
        exit(EXIT_FAILURE);
    }
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
    }
    return 0;
}

