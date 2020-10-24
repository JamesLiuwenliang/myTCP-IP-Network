#include <stdio.h>
#include <unistd.h>
#include <signal.h>

/*
        int sigaction(int signo,const struct sigaction *act,struct sigaction* oldact);
                   传递信号信息  对应第一个参数的信号处理函数信息   注册的信号处理函数指针
        struct sigaction{
            void (*sa_handler)(int);
            sigset_t sa_mask;
            int sa_flags;
        }
*/
void timeout(int sig){
    if(sig==SIGALRM){
        puts("Time out..");
    }

    alarm(2);
}

void keycontrol(int sig){
    if(sig==SIGINT){
        puts("CTRL+C pressed");
    }
}

int main(int argc ,char * argv[]){
    int i ;

    struct sigaction act;
    act.sa_handler = timeout;

    // 这个函数将sa_mask成员的所有位初始化为0
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGALRM, &act,0);
    signal(SIGINT,keycontrol);

    alarm(2);

    for(i=0;i<3;i++){
        puts("wait...");
        sleep(100);
    }
    return 0;

 }
