#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>

/*
 * 利用标准IO库，改善echo函数
 * 实现半关闭,同时单向发送EOF
*/
#define BUF_SIZE 1024
void error_handling(char* message);

int main(int argc,char *argv[]){
    int serv_sock ,clnt_sock;
    char message[BUF_SIZE];

    int str_len ,i;

    struct sockaddr_in serv_adr,clnt_adr;

    socklen_t clnt_adr_sz;

    FILE *readFp;
    FILE *writeFp;

    if(argc != 2){
        printf("Usage : %s <port>\n",argv[0]);
        exit(1);
    }
    

    serv_sock = socket(PF_INET,SOCK_STREAM,0);

    if(serv_sock == -1){
        error_handling("socket() error");
    }

    memset(&serv_adr, 0,sizeof(serv_adr));

    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr* )&serv_adr ,sizeof(serv_adr)) == -1){
        error_handling("bind() error");
    }

    if(listen(serv_sock,5) == -1){
        error_handling("lsiten() error");
    }

    clnt_adr_sz = sizeof(clnt_adr);

    clnt_sock = accept(serv_sock , (struct sockaddr*)&clnt_adr , &clnt_adr_sz );

    readFp = fdopen(clnt_sock,"r");
    // 直接将dup()出来的文件描述符转换为FILE指针给writeFp
    writeFp =fdopen(dup(clnt_sock),"w");

    fputs("Hello World\n",writeFp);
    fputs("Hello World\n",writeFp);
    fputs("Hello World\n",writeFp);
    fflush(writeFp);

    // 关闭输出
    shutdown(fileno(writeFp),SHUT_WR);
    fclose(writeFp);

    // 依然可以获得信息
    fgets(message,sizeof(message),readFp);
    fputs(message,stdout);       

    fclose(readFp);
    return 0;
}




void error_handling(char* message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1) ;
}