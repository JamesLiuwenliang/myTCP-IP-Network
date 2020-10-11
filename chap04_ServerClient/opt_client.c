#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define OPSZ 4
#define RESULT_SIZE 4
void error_handling(char* message);

int main(int argc,char *argv[]){
    int sock;
    char message[BUF_SIZE];

    int numbers[10];
    int count_numbers =0,result=0;
    int i;
    int str_len , recv_len , recv_cnt;
    struct sockaddr_in serv_addr;

    if(argc != 3){
        printf("Usage :%s <IP> <port>\n", argv[0]);
    }

    sock = socket(PF_INET , SOCK_STREAM , 0);
    if(sock == -1){
        error_handling("socket() error");
    }

    memset(&serv_addr , 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&serv_addr , sizeof(serv_addr)) == -1){
        error_handling("connect() error");
    }else{
        puts("Connected ....");
    }


    fputs("Operand count: ",stdout);
    scanf("%d",&count_numbers);

    message[0] = (char)count_numbers;

    for(i=0;i<count_numbers;i++){
        printf("Operand %d: ",i+1);
        scanf("%d",(int*)&message[i*OPSZ+1]);
    }

    fgetc(stdin);
    fputs("Operation: ",stdout);
    scanf("%c",&message[OPSZ*count_numbers+1]);

    write(sock,message,OPSZ*count_numbers+2);
    read(sock,&result,RESULT_SIZE);    

    printf("Operation result: %d\n",result);


        

    close(sock);
    return 0;



}

void error_handling(char* message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1) ;
}
