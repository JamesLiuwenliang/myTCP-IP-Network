#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

// 实现多播
// 需要添加经过加入多播组的过程
#define BUF_SIZE 30
void error_handling(char* message);

int main(int argc ,char* argv[]){
    
    int recv_sock;
    int str_len ; 
    struct sockaddr_in adr;
    struct ip_mreq join_adr;
    char message[BUF_SIZE];

    if(argc != 3){
        printf("Usage : %s <IP> <port>\n",argv[0]);
        exit(1);
    }

    // 这里和服务端不太一样，是要传入IP和端口的
    recv_sock = socket(PF_INET,SOCK_DGRAM,0);
    memset(&adr , 0,sizeof(adr));
    adr.sin_family = AF_INET;
    adr.sin_addr.s_addr = htonl(INADDR_ANY);
    adr.sin_port = htons(atoi(argv[2]));

    if(bind(recv_sock, (struct sockaddr*)&adr,sizeof(adr)) == -1){
        error_handling("bind() error.");
    }

    join_adr.imr_multiaddr.s_addr = inet_addr(argv[1]);
    join_adr.imr_interface.s_addr = htonl(INADDR_ANY);

    setsockopt(recv_sock ,IPPROTO_IP,IP_ADD_MEMBERSHIP ,(void *)&join_adr,sizeof(join_adr));
    
    while(1){
        str_len = recvfrom(recv_sock , message , BUF_SIZE-1 ,0,NULL,0);
        if(str_len < 0){
            break;
        }
        message[str_len] = 0;
        fputs(message,stdout);
    }

    close(recv_sock);
    return 0;

}

void error_handling(char* message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1) ;
}
