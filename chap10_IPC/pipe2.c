#include <stdio.h>
#include <unistd.h>

#define BUF_SIZE 30

int main(int argc,char *argv[]){

    char str1[] = "Hello";
    char str2[] = "World";
    char buf[BUF_SIZE];

    // 文件描述符
    // 用作管道的出入口，fds[0]是管道的出口，fds[1]是管道的入口
    int fds[2];

    pid_t pid;

    pipe(fds);
    // 这个fork()会复制文件描述符
    pid = fork();

    //  单管道下，先read的进程会把数据取走
    // 子进程通过文件描述符fds[1]进入管道传递到父进程的fds[0]
    if(pid == 0){
        // 子进程通过管道传递字符串
        write(fds[1],str1,sizeof(str1));
        // 倘若注释掉这行,会引发错误,因为这次只创造了一个管道
        // 子进程write结束后，直接read管道内的内容，并打印str1的内容，而父进程因为管道内无内容会卡在read处
        sleep(2);
        read(fds[0],buf,BUF_SIZE);
        printf("Child proc output:%s\n",buf);

    }else{

        read(fds[0],buf,BUF_SIZE);
        printf("Father proc output:%s\n",buf);
        write(fds[1],str2,sizeof(str2));
        // 如果注释掉这行，父进程会先终止，所以会弹出命令行提示符，这时子进程再弹出自己的语句
        sleep(2);

    }



    return 0;


}