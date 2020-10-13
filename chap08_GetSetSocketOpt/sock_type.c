#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>


// 查看套接字类型信息
// 套接字类型只能在创建时决定，以后不能更改
// SO_TYPE:用于验证套接字类型，是典型的只读可选项
void error_handling(char* message);

int main(int argc,char* argv[]){

    int tcp_sock,udp_sock;
    int sock_type;
    socklen_t optlen;
    int state;

    optlen = sizeof(sock_type);
    tcp_sock = socket(PF_INET, SOCK_STREAM,0);
    udp_sock = socket(PF_INET, SOCK_DGRAM,0);
    printf("SOCK_STREAM : %d\r\n",SOCK_STREAM); // 1
    printf("SOCK_DGRAM : %d\r\n",SOCK_DGRAM);   // 2

    /* 获取套接字类型信息，如果是TCP套接字，将获得SOCK_STREAM常数值1；如果是UDP套接字，则获得SOCK_DGRAM的常数值2
     * int getsockopt(int sock,int level,int optname , *optval  ,socklen_t *optlen);
     *                        要查看的协议层；可选项名；保存查看结果的缓冲地址值；缓冲大小，最后返回的是可选项信息的字节数
     * */
    state = getsockopt(tcp_sock , SOL_SOCKET,SO_TYPE ,(void *)&sock_type, &optlen);
    if(state){
        error_handling("getsocketopt() error.\r\n");
        exit(1);
    }
    printf("Socket type one : %d\n",sock_type); // 1
    printf("Socket optlen : %d\n",optlen); 


    state = getsockopt(udp_sock,SOL_SOCKET,SO_TYPE,(void *)&sock_type, &optlen);
    if(state){
        error_handling("getsocketopt() error.\r\n");
        exit(1);
    }
    printf("Socket type two : %d\n",sock_type); // 2
    printf("Socket optlen : %d\n",optlen); 


    return 0;

}

void error_handling(char* message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1) ;
}