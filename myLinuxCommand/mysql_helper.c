#include <stdio.h>
#include <mysql/mysql.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define DBHOST "127.0.0.1"
#define DBPORT 3306
#define DBUSER "chuancey"
#define DBPASSWD "Zhangsan@elem123"
#define DBNAME "file_sharing_system"

int main()
{
    MYSQL* mysql = mysql_init(NULL);
    assert(mysql!=NULL);
    unsigned int timeout = 3000;
    mysql_options(mysql,MYSQL_OPT_CONNECT_TIMEOUT,&timeout);
    if(mysql_real_connect(mysql,DBHOST,DBUSER,DBPASSWD,DBNAME,DBPORT,NULL,0)==NULL){
        fprintf(stderr,"Failed to connect to database: Error: %s\n",mysql_error(mysql));
        exit(-1);
    }
    puts("connect successfully!");
    
    const char* sql_query_string="SHOW TABLES";
    if(mysql_real_query(mysql,sql_query_string,strlen(sql_query_string))!=0){
        fprintf(stderr,"query failed: %s\n",mysql_error(mysql));
        exit(-1);
    }

    MYSQL_RES* res = mysql_store_result(mysql);
    if(res==NULL){
        fprintf(stderr,"fetch result failed: %s\n",mysql_error(mysql));
        exit(-1);
    }

    unsigned int num_field = mysql_num_fields(res);
    printf("numbers of result: %u(mysql_num_fileds)\n",num_field);
    MYSQL_FIELD* field = NULL;
    while((field=mysql_fetch_field(res))!=NULL)
        printf("field name: %s\n",field->name);

    MYSQL_ROW row=NULL;
    while((row=mysql_fetch_row(res))!=NULL){
        unsigned long* length = mysql_fetch_lengths(res);
        for(int i=0;i<num_field;i++){
            printf("[%.*s]",(int)length[i],row[i]?row[i]:"NULL");
        }
    }
    printf("\n");


    mysql_free_result(res); //释放结果集
    mysql_close(mysql); //关闭与mysql服务器的连接
    mysql_library_end(); //关闭MySQL数据库的使用
    return 0;
}

