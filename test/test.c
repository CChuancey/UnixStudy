#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

#define PATHLEN 300
void showFileStat(struct stat* fileStat) {
    //输出文件类型
    if(S_ISREG(fileStat->st_mode)) putchar('-');
    if(S_ISDIR(fileStat->st_mode)) putchar('d');
    if(S_ISCHR(fileStat->st_mode)) putchar('c');
    if(S_ISBLK(fileStat->st_mode)) putchar('b');
    if(S_ISFIFO(fileStat->st_mode)) putchar('p');
    if(S_ISLNK(fileStat->st_mode)) putchar('l');
    if(S_ISSOCK(fileStat->st_mode)) putchar('s');

    //owner权限
    int count=0;
    if((S_IRUSR & fileStat->st_mode)==S_IRUSR) {
        putchar('r');
    }else putchar('-');
    if((S_IWUSR & fileStat->st_mode)==S_IWUSR) {
        putchar('w');
    }else putchar('-');

    if((S_IXUSR & fileStat->st_mode)==S_IXUSR) {
        putchar('x');
    }else putchar('-');

    //group权限    
    if((S_IRGRP & fileStat->st_mode)==S_IRGRP) {
        putchar('r');
    }else putchar('-');

    if((S_IWGRP & fileStat->st_mode)==S_IWGRP) {
        putchar('w');
    }else putchar('-');

    if((S_IXGRP & fileStat->st_mode)==S_IXGRP) {
        putchar('x');
    }else putchar('-');

    //other权限
    if((S_IROTH & fileStat->st_mode)==S_IROTH) {
        putchar('r');
    }else putchar('-');

    if((S_IWOTH & fileStat->st_mode)==S_IWOTH) {
        putchar('w');
    }else putchar('-');

    if((S_IXOTH & fileStat->st_mode)==S_IXOTH) {
        putchar('x');
    }else putchar('-');

    // inode数
    printf(" %lu ",fileStat->st_nlink);

    // 文件拥有者用户名和组名
    struct passwd* upwd = getpwuid(fileStat->st_uid);
    printf("%s ",upwd->pw_name);
    struct group* gpwd = getgrgid(fileStat->st_gid);
    printf("%s ",gpwd->gr_name);

    // 文件大小
    printf("%-5ld ",fileStat->st_size);

    // 最后一次修改时间
    printf("%-12.12s ",ctime(&fileStat->st_ctime)+4);

}

int main(int argc,char** argv) {
    char path[PATHLEN];
    if(argc==1){
        strcpy(path,".");
    } else {
        strcpy(path,argv[1]);
    }
    DIR* dir=opendir(path);
    if(dir==NULL) {
        perror("opendir()");
        exit(1);
    }

    struct dirent* item;
    errno = 0;
    struct stat statBuffer;
    while((item=readdir(dir))!=NULL) {
        char filePath[PATHLEN*2];
        sprintf(filePath,"%s/%s",path,item->d_name);
        if(stat(filePath,&statBuffer)==-1) {
            perror("stat()");
            exit(1);
        }
        if(strcmp(".",item->d_name)==0||strcmp("..",item->d_name)==0) {
            continue;
        }
        showFileStat(&statBuffer);
        printf("%s\n",item->d_name);
    }

    if(errno!=0) {
        perror("readdir()");
        exit(1);
    }

    closedir(dir);

    return 0;
}
