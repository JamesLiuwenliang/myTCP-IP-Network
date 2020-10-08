#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char* message);

int main(int argc ,char* argv[]){
    
    int serv_sock;
    char message[BUF_SIZE];

    int str_len;

    int clnt_adr_size;
    socklen_t adr_size;
    struct sockaddr_in serv_adr, clnt_adr;

    if(argc == 3){
        printf("Usage : %s <IP> <port>\n",argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET,SOCK_DGRAM,0);
    if(serv_sock == -1){
        error_handling("UDP socket creation error.");
    }

    memset(&serv_adr , 0,sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));
    
    if(bind(serv_sock, (struct sockaddr*)&serv_adr ,sizeof(serv_adr)) == -1){
        error_handling("bind() error");
    }

    while(1){
        clnt_adr_size = sizeof(clnt_adr);
        /* 基于UDP的数据IO函数
         * ssize_t recvfrom(int sock,void *buff ,size_t nbytes,int flags ,struct sockaddr *from,socklen_t *addrlen);
         * 接收的字节数 ；                      可接受的最大字节数；  传0； 存有发送端地址信息的sockaddr结构体变量的地址值             
         */
        str_len = recvfrom(serv_sock, message ,BUF_SIZE ,0,(struct sockaddr*)&clnt_adr ,&clnt_adr_size);

        /*ssize_t sendto(int sock ,void *buff ,size_t nbytes,int flags ,struct sockaddr *to,socklen_t addrlen);
         * 传输的字节数；                      待传输的数据长度； 传0；存有目标地址信息的sockaddr结构体
         */
        sendto(serv_sock , message , str_len , 0 , (struct sockaddr*)&clnt_adr,clnt_adr_size);
    }

    close(serv_sock);
    return 0;

}

void error_handling(char* message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1) ;
}
