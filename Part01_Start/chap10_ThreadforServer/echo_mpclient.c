#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char* message);
void write_process(int scok ,char *message);
void read_process(int sock,char *message);
void write_childproc(int sig);

int main(int argc,char *argv[]){

    int sock,state;
    char message[BUF_SIZE];
    pid_t pid;
    struct sockaddr_in serv_addr;

    struct sigaction act;
    // 声明信号,这4行都是为了防止僵尸进程
    act.sa_handler = write_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    state = sigaction(SIGCHLD,&act,0);


    if(argc != 3){
        printf("Usage :%s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET , SOCK_STREAM , 0);
    if(sock == -1){
        error_handling("socket() error");
    }
    memset(&serv_addr , 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&serv_addr , sizeof(serv_addr)) == -1){
        error_handling("connect() error");
    }else{
        puts("Connected ....");
    }

    pid = fork();
    if(pid==0){
        write_process(sock,message);
    }else{
        read_process(sock,message);
    }

    close(sock);
    return 0;

}

void write_process(int sock,char *message){
    
    while(1){

        fgets(message , BUF_SIZE,stdin);
        if(!strcmp(message, "q\n") || !strcmp(message,"Q\n")){

            // 向服务器返回EOF
            shutdown(sock,SHUT_WR);
            return;
        }
        
        write(sock,message,strlen(message));


    }
}

void read_process(int sock,char *message){

    int str_len;

    while(1){

        int str_len = read(sock ,message,BUF_SIZE-1);

        // 这里为什么必须要return;
        if(str_len == -1){
            return;
        }

        message[str_len] = 0;
        printf("Message from server: %s\r\n",message);
        
    }
}

void write_childproc(int sig){
    pid_t pid ;
    int status ;
    pid = waitpid(-1,&status,WNOHANG);
    printf("Remove child pid is : %d\n",pid);
}

void error_handling(char* message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1) ;
}
