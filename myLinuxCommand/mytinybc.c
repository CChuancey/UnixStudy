#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFFR_SIZE 1024
#define exitErr(func) {perror(func);exit(EXIT_FAILURE);}

void childFunc(int in[2],int out[2]){
    close(in[1]);
    close(out[0]);
    char buffer[BUFFR_SIZE];
    FILE* fin = fdopen(in[0],"r");
    FILE* fout = fdopen(out[1],"w");
    if(fin==NULL||fout==NULL) exitErr("fdopen()");
    int num1,num2;
    char op[BUFFR_SIZE];
    while(printf("tinybc:"),fgets(buffer,BUFFR_SIZE,stdin)!=NULL){
        if(sscanf(buffer,"%d%[-+*/^]%d\n",&num1,op,&num2)!=3){
            puts("Syntax error!");
            continue;
        }
        if(fprintf(fout,"%d\n%d\n%c\np\n",num1,num2,*op)<0){ //转换为逆波兰表达式
            fprintf(stderr,"Error fprintf()\n");
            exit(1);
        }
        fflush(fout);
        printf("%d %c %d=\n",num1,*op,num2);
        while(fgets(buffer,BUFFR_SIZE,fin)!=NULL)//阻塞读入跳不出循环！
            printf("%s",buffer);
        //fflush(stdout);
        //if(fgets(buffer,BUFFR_SIZE,fin)==NULL) break;
        //printf("%d %s %d = %s",num1,op,num2,buffer);
    }
    fclose(fin);
    fclose(fout);
}


void parentFunc(int in[2],int out[2]){
    close(in[0]);
    close(out[1]);
    if(dup2(in[1],1)==-1||dup2(out[0],0)==-1) exitErr("dup2()");
    execlp("dc","dc","-",NULL); //dc 的-表示从标准输入读入
    exitErr("execlp()");
}

int main(int argc,char** argv)
{
    int pipefrm[2],pipeto[2];
    if(pipe(pipefrm)==-1||pipe(pipeto)==-1){
        exitErr("pipe()");
    }
    pid_t pid = fork();
    if(pid==-1){
        exitErr("fork()");
    } else if(pid==0){//child to format input
        childFunc(pipefrm,pipeto);
    } else {//parent to calculate
        parentFunc(pipefrm,pipeto);
    }
    return 0;
}

