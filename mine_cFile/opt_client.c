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

    int numbers[12] = {0};

    char message[BUF_SIZE];
    char *message_getIn = malloc(4) ;
    int count_numbers =0,result=0 , tmp_In=-1;
    int i=0;
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


    while(i<10){
        printf("In %d: ",i+1);
        // message_getIn
        gets(message_getIn);
        tmp_In = atoi(message_getIn);
        if(tmp_In == 0){
            break;
        }
        numbers[1+ i++] = tmp_In;
        
    }
    // 放总数的
    numbers[0] = i;

    if( !strcmp(message_getIn,"+") || !strcmp(message_getIn,"-") || !strcmp(message_getIn,"*") || !strcmp(message_getIn,"/")){     

        switch (*message_getIn)
        {
            case '+':
                numbers[i+1] = -1;
                break;
            case '-':
                numbers[i+1] = -2;
                break;
            case '*':
                numbers[i+1] = -3;
                break;
            case '/':
                numbers[i+1] = -4;
                break;
            default:
                numbers[i+1] = -5;
                break;

        }
    
    }


    write(sock,numbers, 48);
    
    
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
