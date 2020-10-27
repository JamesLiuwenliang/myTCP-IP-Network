#include <stdio.h>
#include <pthread.h>
/*因为共用sum空间,可能会有临界区风险*/
void *thread_summation(void *arg);
int sum = 0;
int main(int argc ,char *argv[]){
    pthread_t id_t1,id_t2;
    int range1[] = {1,5};
    int range2[] = {6,10};

    pthread_create(&id_t1,NULL,thread_summation , (void *)range1);
    pthread_create(&id_t2,NULL,thread_summation , (void *)range2);

    pthread_join(id_t1, NULL);
    pthread_join(id_t2, NULL);

    printf("result : %d\n",sum);
}

void *thread_summation(void *arg){
    int i;

    int startNum = ((int *)arg)[0];
    int endNum = ((int *)arg)[1];

    while(startNum <= endNum){
        sum += startNum++;
    }


}
