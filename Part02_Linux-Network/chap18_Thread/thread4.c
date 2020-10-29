#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
/*因为共用sum空间,可能会有临界区风险*/
#define NUM_THREAD 100
void *thread_inc(void *arg);
void *thread_des(void *arg);
long long num = 0;

pthread_mutex_t mutex;

/*
 * 互斥量：通过加锁，解决线程同步问题
 */

int main(int argc ,char *argv[]){

    pthread_t thread_id[NUM_THREAD];

    int i;

    // 声明了保存互斥量读取值的变量
    pthread_mutex_init(&mutex,NULL);

    for(i=0;i<NUM_THREAD;i++){
        if(i%2){
            pthread_create(&(thread_id[i]) ,NULL, thread_inc,NULL);
        }else{
            pthread_create(&(thread_id[i]), NULL, thread_des,NULL);
        }       
    }

    for(i=0;i<NUM_THREAD;i++){
        pthread_join(thread_id[i],NULL);
    }

    printf("result : %lld\n",num);

    // 销毁互斥量
    pthread_mutex_destroy(&mutex);
    return 0;
}


// 应最大限度的减少调用锁的次数,thread_dex()明显比thread_inc()调用次数要多很多次,所以运行次数很慢
void *thread_inc(void *arg){

    int i;
    pthread_mutex_lock(&mutex);
    for(i=0;i<500000;i++){
        num += 1;
    }
    pthread_mutex_unlock(&mutex);
    return NULL;
}
void *thread_des(void *arg){
    int i;
    
    for(i=0;i<500000;i++){
        pthread_mutex_lock(&mutex);
        num -= 1;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;


}
