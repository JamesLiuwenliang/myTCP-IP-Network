#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>


void read_childproc(int sig){
    int status ;
    // 传递-1可以等待任意子进程结束，这样即使没有进入终止状态的子进程，也不会进入阻塞状态
    pid_t id = waitpid(-1,&status ,WNOHANG);
    if(WIFEXITED(status)){
        printf("Removed proc id :%d\r\n",id);
        printf("Child send: %d\r\n",WEXITSTATUS(status));
    }
}

int main(int argc,char *argv[]){
    pid_t pid;
    struct sigaction act;
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGCHLD, &act,0);

    pid = fork();
    if(pid == 0){

        puts("I'm first child process\r\n");
        sleep(5);
        return 12;

    }else{

        printf("The first child's pid is %d\r\n",pid);
        printf("I'm parent process.Now I'll create 2nd child process .\r\n ");
        pid = fork();
        if(pid == 0){
            puts("I'm second child's process");
            sleep(5);
            exit(24);
        }else{
            int i;
            printf("I'm parent process . The second child's process pid is %d \r\n",pid);
            for(i =0;i<8;i++){
                puts("wait...");
                sleep(5);
            }
        }



    }

    return 0;
}




