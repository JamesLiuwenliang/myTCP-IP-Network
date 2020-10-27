#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
void *thread_main(void *arg);

/*
控制线程的执行流: 在执行pthread_join()后，主线程会暂停,进入thread_main中执行
执行return (void *)msg;后主线程继续运行

*/

int main (int argc ,char *argv[]){

    pthread_t t_id;
    int thread_param = 5;
    void *thr_ret;

    if(pthread_create(&t_id , NULL , thread_main , (void *)&thread_param) != 0){
        puts("thread_create() error");
        return 0;
    }

    // main函数将等待ID保存到t_id变量中的线程终止(不需要上次的sleep())
    if(pthread_join(t_id, &thr_ret) != 0 ){
        puts("pthread_join() error");
        return 0;
    }

    printf("Thread return message :%s\n",(char *)thr_ret);

    // thr_ret作为返回值,返回的空间是在thread_main中的新开辟的内存，所以需要free()掉
    free(thr_ret);
    return 0;

}

void *thread_main(void *arg){

    int i;
    int cnt = * ((int *)arg);

    char *msg = (char *)malloc(sizeof(char) * 50);
    strcpy(msg ,"Hello ,I'm thread!\n");

    for(i=0;i<cnt ; i++){
        sleep(1);
        puts("Thread is running ");
    }

    return (void *)msg;

}
