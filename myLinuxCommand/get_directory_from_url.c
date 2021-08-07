#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>


char* get_directory_from_url(char* url){//返回值不是/的情况要free掉返回值
   int len = strlen(url);
   len--;
   while(url[len]!='/') len--;
   char* res = (char*)malloc(len+1);
   memcpy(res,url,len+1);
   return res;
}

int main()
{
    char url[1024];
    fgets(url,1024,stdin);
    char* res = get_directory_from_url(url);
    puts(res);
    if(strcasecmp(url,"/")!=0) free(res);
    return 0;
}

