#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
/**
 *  信号量
*/

void *read(void *arg);
void *accu(void *arg);

static sem_t sem_one; // 为了防止调用read()的线程写入数值前，accu()取走数据
static sem_t sem_two; // 为了防止在调用accu()的线程还未取走数据的情况下,调用read()的线程覆盖原值
static int num ;

int main(int argc,char *argv[]){

    pthread_t id_t1, id_t2;
    sem_init(&sem_one, 0,0);
    sem_init(&sem_two, 0,1);

    pthread_create(&id_t1,NULL,read,NULL);
    pthread_create(&id_t2,NULL,accu,NULL);

    pthread_join(id_t1, NULL);
    pthread_join(id_t2, NULL);

    sem_destroy(&sem_one);
    sem_destroy(&sem_two);

    return 0;
}

void *read(void *arg){
    int i;
    for(i=0;i<5;i++){
        fputs("Input num :",stdout);
        sem_wait(&sem_two);
        scanf("%d",&num);
        sem_post(&sem_one);
    }
    return NULL;
}
void *accu(void *arg){
    int i ,sum=0;
    for(i=0;i<5;i++){
        sem_wait(&sem_one);
        sum += num;

        sem_post(&sem_two);
    }
    
    printf("Output num : %d ",sum);
    return NULL;


}