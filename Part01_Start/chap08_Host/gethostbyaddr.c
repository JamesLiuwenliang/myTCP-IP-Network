#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

void error_handling(char* message);

int main(int argc,char* argv[]){

    int i;
    struct hostent *host;
    struct sockaddr_in addr;

    if(argc != 2){
        printf("Usage : %s <port>\n",argv[0]);
        exit(1);
    }

    memset(&addr,0,sizeof(addr));
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    host = gethostbyaddr((char *)&addr.sin_addr,4,AF_INET);
    if(!host){
        error_handling("gethost error");
    }


    // 官方域名 ： www.baidu.com
    printf("Official name :%s \n",host->h_name);

    // 其他域名 ： Aliases 1 :www.baidu.com 
    //            Aliases 2 :www.a.shifen.com 
    for(i=0;host->h_aliases[i];i++){
        printf("Aliases %d :%s \n",i+1,host->h_aliases[i]);
    }

    // IP地址的地址族信息 ： IPV4
    printf("Address type : %s \n",(host->h_addrtype==AF_INET)?"AF_INET":"AF_INET6");

    // IP地址    IP addr 1: 119.63.197.151
    //           IP addr 2: 119.63.197.139
    for(i=0;host->h_addr_list[i];i++){
        printf("IP addr %d: %s\n",i+1,inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));
    }

    return 0;

}



void error_handling(char* message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1) ;
}