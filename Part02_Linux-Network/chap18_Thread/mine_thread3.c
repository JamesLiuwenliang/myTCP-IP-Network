#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int sum =0;
void * thread_summation(void *arg);

/*
 * 保证线程安全可通过编译时添加 `-D_REENTRANT `选项定义宏
 * $ gcc -D_REENTRANT mythread.c -o mthread -lpthread
 * 
 * */
int main(int argc, char *argv[]){

    pthread_t t_id_01,t_id_02;

    void *thr_ret;
    int num = 0;
    void *p_sum = (void *)&sum;

    if( ( pthread_create(&t_id_01 ,NULL ,thread_summation,(void *)&num) && pthread_create(&t_id_02,NULL,thread_summation , (void *)&num) )!= 0){
        puts("thread_create() error");
        return -1;
    }

    if(pthread_join(t_id_01 , &p_sum) != 0){
        puts("pthread_join() for t_id_01 error");
        return -1;
    }

    printf("Thread01 return message : %d\n",*(int *)p_sum);
    sum = 0;
    num = 5;

    if(pthread_join(t_id_02,&p_sum) != 0){
        puts("pthread_join() for t_id_02 error");
        return -1;
    }

    printf("Thread02 return message : %d\n",*(int *)p_sum);
    sum =0;
    free(p_sum);
    return 0;


}


void *thread_summation(void *arg){

    int i;
    int cnt = 5;
    
    int *pNum = (int *)malloc(sizeof(int));
    *pNum = 0;

    int startNum = *((int *)arg);

    for(i=0;i<cnt ;i++){
        *pNum += startNum+1;
        startNum++; 
    }

    
    
    return (void *)pNum;


}

