#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

// 实现多播
#define TTL 64
#define BUF_SIZE 30
void error_handling(char* message);

int main(int argc ,char* argv[]){
    
    int send_sock; 
    struct sockaddr_in mul_adr;

    char message[BUF_SIZE];

    FILE *fp;
    int time_live = TTL;

    if(argc != 3){
        printf("Usage : %s <IP> <port>\n",argv[0]);
        exit(1);
    }

   // 这里和服务端不太一样，是要传入IP和端口的
    send_sock = socket(PF_INET,SOCK_DGRAM,0);
    memset(&mul_adr , 0,sizeof(mul_adr));
    mul_adr.sin_family = AF_INET;
    // 这里必须将IP地址设置为多播地址
    mul_adr.sin_addr.s_addr = inet_addr(argv[1]);
    mul_adr.sin_port = htons(atoi(argv[2]));
    
    // 设置TTL
    // IPPROTO_IP：设置TTL相关协议层，选项名
    setsockopt(send_sock ,IPPROTO_IP , IP_MULTICAST_TTL, (void *)&time_live ,sizeof(time_live));
    if((fp = fopen("news.txt","r")) == NULL){
        error_handling("fopen() error.");
    }

    // Broadcasting
    while( !feof(fp) ){
        fgets(message,BUF_SIZE,fp);
        // 多播利用UDP实现，所以需要sendto()函数
        sendto(send_sock,message,strlen(message) , 0,(struct sockaddr*)&mul_adr,sizeof(mul_adr));
        sleep(2);
    }


    fclose(fp);
    close(send_sock);
    return 0;

}

void error_handling(char* message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1) ;
}
