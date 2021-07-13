#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <unistd.h>

//任务链表
typedef struct Worker{
    void* (*process)(void* arg);
    void* arg;
    struct Worker* next;
}CThread_worker;

//线程池结构
typedef struct{
    pthread_mutex_t queue_lock;
    pthread_cond_t queue_ready;//条件变量
    CThread_worker* queue_head;
    int shutdown;
    pthread_t* threadid;
    int max_thread_num;
    int cur_queue_size;
}CThread_pool;

void pool_init(int);//初始化线程池
int pool_add_worker(void* (*process)(void* arg),void*);
int pool_destory(); //销毁线程池
void* thread_routine(void*);//工作线程
void* func(void*);//测试任务

static CThread_pool* thread_pool = NULL;

void pool_init(int max_thread_num){
    thread_pool = (CThread_pool*)malloc(sizeof(CThread_pool));
    pthread_mutex_init(&thread_pool->queue_lock,NULL);
    pthread_cond_init(&thread_pool->queue_ready,NULL);
    thread_pool->queue_head=NULL;
    thread_pool->shutdown=0;
    thread_pool->threadid = (pthread_t*)malloc(sizeof(pthread_t) * max_thread_num); //分配够最大线程数的内存空间,无法预先指定只好动态分配,此处为指针空间并非线程空间
    for(int i=0;i<max_thread_num;i++){
        pthread_create(&(thread_pool->threadid[i]),NULL,thread_routine,NULL);
    }
    thread_pool->max_thread_num = max_thread_num;
    thread_pool->cur_queue_size = 0;
}

int pool_add_worker(void* (*process)(void* arg),void* arg){
    CThread_worker* worker = (CThread_worker*)malloc(sizeof(CThread_worker));
    worker->process = process;
    worker->arg = arg;
    worker->next = NULL; //不带头节点的尾插法，加NULL，FIFO

    pthread_mutex_lock(&thread_pool->queue_lock); //互斥变量上锁
    CThread_worker* member = thread_pool->queue_head;
    if(member==NULL){
        thread_pool->queue_head = worker;
    }else{
        while(member->next!=NULL){
            member=member->next;
        }
        member->next=worker;
    }
    assert(thread_pool->queue_head!=NULL); //头节点一定不为NULL
    thread_pool->cur_queue_size++;
    pthread_mutex_unlock(&(thread_pool->queue_lock));//解开互斥锁
    pthread_cond_signal(&(thread_pool->queue_ready));//唤醒等待线程
    return 0;
}

int pool_destory(){
    //防止重复调用
    if(thread_pool->shutdown) return -1;
    thread_pool->shutdown=1;

    //先唤醒所有等待线程，再用pthread_join防止产生僵尸线程
    pthread_cond_broadcast(&thread_pool->queue_ready);
    for(int i=0;i<thread_pool->max_thread_num;i++){
        pthread_join(thread_pool->threadid[i],NULL);
    }
    free(thread_pool->threadid);

    //销毁等待队列
    while(thread_pool->queue_head!=NULL){
        CThread_worker* worker = thread_pool->queue_head->next;
        free(thread_pool->queue_head);
        thread_pool->queue_head = worker;
    }

    //销毁条件变量和互斥量
    pthread_mutex_destroy(&thread_pool->queue_lock);
    pthread_cond_destroy(&thread_pool->queue_ready);

    free(thread_pool);
    thread_pool=NULL; //销毁后置空是好习惯
    return 0;
}

void* thread_routine(void* arg){
    printf("starting thread 0x%lx\n",pthread_self());
    while(1){
        pthread_mutex_lock(&thread_pool->queue_lock);
        while(thread_pool->cur_queue_size==0&&!thread_pool->shutdown){
            printf("thread 0x%lx is waiting\n",pthread_self());
            pthread_cond_wait(&(thread_pool->queue_ready),&(thread_pool->queue_lock));              
        }
        if(thread_pool->shutdown){
            //要销毁线程池，先解锁
            pthread_mutex_unlock(&(thread_pool->queue_lock));
            printf("thread 0x%lx will exit\n",pthread_self());
            pthread_exit(NULL);
        }

        printf("thread 0x%lx is starting working\n",pthread_self());
        assert(thread_pool->cur_queue_size!=0);
        assert(thread_pool->queue_head!=0); //等待队列为空无法工作！

        thread_pool->cur_queue_size--;
        CThread_worker* worker = thread_pool->queue_head;
        thread_pool->queue_head=worker->next;
        pthread_mutex_unlock(&thread_pool->queue_lock);
        (*(worker->process))(worker->arg);
        free(worker);
        worker=NULL;
    }
    pthread_exit(NULL); //实际无法执行
}


void* func(void* s){
    printf("threadid is 0x%lx, working on task %d\n",pthread_self(),*(int *)s);
    sleep(1);
    return NULL;
}

int main()
{
    pool_init(3);

    int* workingnum = (int*)malloc(sizeof(int)*10);
    for(int i=0;i<10;i++){
        workingnum[i]=i;
        pool_add_worker(func,&workingnum[i]);
    }
    sleep(5);
    pool_destory();
    free(workingnum);

    return 0;
}

