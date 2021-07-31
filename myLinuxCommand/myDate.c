#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

//需要自己释放空间
char* get_formated_time_str(){
    time_t t = time(NULL);
    struct tm* buf = localtime(&t);
    char* res = (char*)malloc(sizeof(char)*20);
    sprintf(res,"%d-%d-%d %d:%d:%d",buf->tm_year+1900,buf->tm_mon+1,buf->tm_mday,
            buf->tm_hour+1,buf->tm_min,buf->tm_sec);
    return res;
}

int main()
{
    char* t = get_formated_time_str();
    puts(t);
    free(t);
    return 0;
}

