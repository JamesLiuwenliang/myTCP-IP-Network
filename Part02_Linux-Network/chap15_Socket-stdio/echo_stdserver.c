#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>

/*
 * 利用标准IO库，改善echo函数
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



    
    
    for(i=0 ; i<5 ;i++){

        clnt_sock = accept(serv_sock , (struct sockaddr*)&clnt_adr , &clnt_adr_sz );

        if(clnt_sock == -1){
            error_handling("accept() error");
        }else{
            printf("Connect client :%d\n",i+1);
        }


        readFp = fdopen(clnt_sock,"r");
        writeFp =fdopen(clnt_sock,"w");

        while( !feof(readFp) ){
            // readFp,writeFp为文件指针
            fgets(message,BUF_SIZE,readFp);
            fputs(message,writeFp);
            // 标准IO提供缓冲的原因，如果不调用fflush(),无法保证一定传到客户端
            fflush(writeFp);
        }

        fclose(readFp);
        fclose(writeFp);

    }

    close(serv_sock);
    return 0;
}




void error_handling(char* message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1) ;
}