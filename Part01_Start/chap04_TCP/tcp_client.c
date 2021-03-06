#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char *message);

int main(int argc,char* argv[]){

    int sock;
    struct sockaddr_in serv_addr;
    char message[30];
    int str_len = 0;
    int idx = 0,read_len=0;

    if(argc!=3){
        printf("Usage : %s <IP> <Port>\n",argv[0]);
        exit(1);
    }

    /* int socket(int domain,int type,int protocol);
     * IPv4,TCP : socket(PF_INET,sock_STREAM,IPPROTO_TCP)
     * IPv4,UDP : socket(PF_INET,SOCK_DGRAM,IPPROTO_UDP)
     * 若前两个参数是PF_INET,sock_STREAM，则可以省略第三个参数
     */
    sock = socket(PF_INET,SOCK_STREAM,0);
    
    if(sock==-1){
        error_handling("socket() error");
    }

    memset(&serv_addr,0,sizeof(serv_addr));

    // inet_addr()函数可以将IP地址转换成为32位整数型数据，在数据类型转换的同时进行网络字节序的转换
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]);

    if(connect(sock, (struct sockaddr*)&serv_addr ,sizeof(serv_addr))==-1 ){
        error_handling("connect() error!");
    }

    /**
     * while循环中每次读取一个字节，直到read()返回0
     * read_len 的值应该是始终为1
     */
    while(read_len = read(sock , &message[idx++],1 )){
        if(read_len == -1){
            error_handling("read() error!");        
        }

        str_len += read_len;
    }

    printf("Message from server :%s \n",message);
    printf("Function read call count :%d \n",str_len);
    close(sock);
    return 0;

}

void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}