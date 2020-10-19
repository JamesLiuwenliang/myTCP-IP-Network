#include <stdio.h>
#include <unistd.h>

#define BUF_SIZE 30

int main(int argc,char *argv[]){

    char str1[] = "Hello";
    char str2[] = "World";
    char buf[BUF_SIZE];

    // 文件描述符
    int fds1[2],fds2[2];

    pid_t pid;

    pipe(fds1),pipe(fds2);
    pid = fork();

    if(pid == 0){

        write(fds1[1],str1,sizeof(str1));
        read(fds2[0],buf,BUF_SIZE);
        printf("Child proc output:%s\n",buf);

    }else{

        read(fds1[0],buf,BUF_SIZE);
        printf("Father proc output:%s\n",buf);
        write(fds2[1],str2,sizeof(str2));
        sleep(2);

    }



    return 0;


}