#include <stdio.h>
#include <unistd.h>

#define BUF_SIZE 30
int main(int argc,char *argv[]){

    char buf[] = "Hello World.";
    char message[BUF_SIZE];

    // 文件描述符
    // 用作管道的出入口，fds[0]是管道的出口，fds[1]是管道的入口
    int fds[2];

    pid_t pid;

    pipe(fds);
    // 这个fork()会复制文件描述符
    pid = fork();

    // 子进程通过文件描述符fds[1]进入管道传递到父进程的fds[0]
    if(pid == 0){
        // 子进程通过管道传递字符串
        write(fds[1],buf,sizeof(buf));

    }else{

        read(fds[0],message,BUF_SIZE);
        puts(message);


    }

    return 0;


}