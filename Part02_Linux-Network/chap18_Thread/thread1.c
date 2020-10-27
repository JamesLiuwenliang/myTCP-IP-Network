#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
void *thread_main(void *arg);

/**
 * Linux 下编译 `gcc thread1.c -o thread1 -lpthread`
 */

int main(int argc,char *argv[]){
    pthread_t t_id;
    int pthread_param = 5;

    /* 线程的创建 
     * int pthread_create(pthread_t *th, const pthread_attr_t *attr, void *(*func)(void *), void *arg);
     * - t_id 是Thread的ID
     * - 传递NULL，为默认属性的线程
     * - thread_main : 相当于thread的main()函数
     * - 包含一些想传递的参数
     */
    if(pthread_create(&t_id ,NULL, thread_main, (void *)&pthread_param ) != 0){
        puts("pthread_create error()");
        return -1;
    }

    sleep(10);
    puts("End of main");
    return 0;

}

void *thread_main(void *arg){

    int i ;
    int cnt = *((int *)arg);
    for(i=0;i<cnt;i++){
        sleep(1);
        puts("Thread is running .");

    }
    return NULL;
}

