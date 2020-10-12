#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

/*
 * 基于半关闭的文件传输程序
 * 服务器端关闭输出流服务后，依然可以收到客户端发来的"Thank you"信息，因为开启了半关闭方法（TCP适用）
*/
#define BUF_SIZE 30
void error_handling(char* message);

int main(int argc,char *argv[]){

    int serv_sock ,clnt_sock;
    FILE *fp;
    char buf[BUF_SIZE];
    int read_cnt;

    struct sockaddr_in serv_adr,clnt_adr;
    socklen_t clnt_adr_sz;

    if(argc != 2){
        printf("Usage : %s <port>\n",argv[0]);
        exit(1);
    }

    // 传输文件
    fp = fopen("file_server.c","rb");

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
    

    printf("speed：");
    while(1){

        read_cnt = fread((void*)buf, 1,BUF_SIZE,fp);
        printf("%d ",read_cnt);

        if(read_cnt < BUF_SIZE){
            write(clnt_sock,buf,read_cnt);
            break;
        }

        write(clnt_sock,buf,BUF_SIZE);
    }

    printf("\r\n");

    // 传输完文件进行半关闭，就是向客户端传输EOF
    // SHUT_WR 只关闭输出流，依然可以通过输入流接收数据
    shutdown(clnt_sock,SHUT_WR);
    read(clnt_sock,buf,BUF_SIZE);
    printf("Message from client: %s \r\n",buf);

    fclose(fp);
    close(clnt_sock);
    close(serv_sock);

    return 0;
}


void error_handling(char* message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1) ;
}