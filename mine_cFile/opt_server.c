#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

/*
 * 实现echo，服务端向客户端发送客户端传来的信息
*/
#define BUF_SIZE 1024
#define OPSZ 4
void error_handling(char* message);
int caculate(int opnum , int opnds[]);

int main(int argc,char *argv[]){

    int serv_sock ,clnt_sock;
    char message[BUF_SIZE];

    int numbers[10]={0};
    int i,recv_cnt , recv_len;

    int result=0, count_numbers = 0;

    struct sockaddr_in serv_adr,clnt_adr;
    socklen_t clnt_adr_sz;

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

        // 操作数
        count_numbers = 0;
        recv_len = 0;

        clnt_sock = accept(serv_sock , (struct sockaddr*)&clnt_adr , &clnt_adr_sz );

        if(clnt_sock == -1){
            error_handling("accept() error");
        }

        // 获得操作数
        read(clnt_sock, &count_numbers ,4);
        printf("Total : %d\r\n",count_numbers);

        for(i=0;i<count_numbers+1;i++){
            read(clnt_sock, &numbers[i],4);
            printf("%d , ",numbers[i]);
        }

        // while( ((count_numbers+1) * OPSZ) > recv_len){
        //     recv_cnt = read(clnt_sock,&message[recv_len],BUF_SIZE-1);
        //     recv_len += recv_cnt;
        // }

        result = caculate(count_numbers,numbers);
        write(clnt_sock,(char*)&result,sizeof(result));         

        close(clnt_sock);
    }
    close(serv_sock);
    return 0;
}


int caculate(int opnum , int opnds[]){

    int tmp = opnds[opnum];
    char op;
    int result = opnds[0],i;
    
    switch(tmp){
        case -1:
            op = '+';break;
        case -2:
            op = '-';break;
        case -3:
            op = '*';break;
        case -4:
            op = '/';break;
        default:
            break;
    }


    switch (op)
    {
    case '+':
        for(i=1;i<opnum;i++){result += opnds[i];}
        break;
    
    case '*':
        for(i=1;i<opnum;i++){result *= opnds[i];}
        break;

    case '-':
        for(i=1;i<opnum;i++){result -= opnds[i];}
        break;

    default:
        break;
    }

    return result;

}




void error_handling(char* message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1) ;
}