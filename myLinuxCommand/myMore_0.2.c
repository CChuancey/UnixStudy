#include <stdio.h>
#include <stdlib.h>

#define PAGELEN 30
#define LINELEN 512

// 动手实现more命令
int main(int argc,char** argv) {
    /*暂时不能实现对参数个数的判断
    if(argc < 2) {
        fprintf(stderr,"Erro:Usage...\n");
        exit(-1);
    }*/
    FILE* fp;
    if(argc == 1) { //暂且不考虑只输入more 的情况
        fp = fdopen(0,"r");
    }else {
        fp = fopen(argv[1],"r");
    }
    if(fp==NULL) {
        perror("fopen():");
        exit(-1);
    }
    int lineCount = 0; 
    while(1) {
        char buffer[LINELEN];
        if(fgets(buffer,LINELEN,fp)==NULL) {
            fclose(fp);
            exit(0);
        }   
        fputs(buffer,stdout);
        lineCount++;
        if(lineCount==PAGELEN) {
            //fprintf(stdout,"----MORE----\n");
            while(1) {
                FILE* tty = fopen("/dev/tty","r");
                char ch = fgetc(tty);
                fgetc(tty);
                switch(ch) {
                    case '\n':
                        if(fgets(buffer,LINELEN,fp)==NULL) {
                            fclose(fp);
                            fclose(tty);
                            exit(0);
                        }
                        fputs(buffer,stdout);           
                        break;
                    case 'h':
                        printf("help docs\n");
                        break;
                    case 'q':
                        exit(0);
                        break;
                }
            }
        }
    }

    return 0;
}
