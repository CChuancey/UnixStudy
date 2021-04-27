#include <stdio.h>
#include <stdlib.h>

#define PAGELEN 30
#define LINELEN 512

// 动手实现more命令
int main(int argc,char** argv) {
    if(argc < 2) {
        fprintf(stderr,"Erro:Usage...\n");
        exit(-1);
    }
    FILE* fp = fopen(argv[1],"r");
    if(fp==NULL) {
        perror("fopen():");
        exit(-1);
    }
    int lineCount = 0; 
    while(1) {
        char buffer[LINELEN];
        if(fgets(buffer,LINELEN,fp)==NULL) {
            exit(0);
        }   
        fputs(buffer,stdout);
        lineCount++;
        if(lineCount==PAGELEN) {
            //fprintf(stdout,"----MORE----\n");
            while(1) {
                char ch = getchar();
                getchar();
                switch(ch) {
                    case '\n':
                        if(fgets(buffer,LINELEN,fp)==NULL) {
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
