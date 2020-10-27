#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <errno.h>
#include <fcntl.h>

/* 正确的以边缘触发方式工作的回声服务器端
 *
 * 边缘触发服务器端实现中必知的两点：
 * - 通过errno变量验证错误的原因
 * - 为了完成非阻塞IO,更改套接字特性
 * 
 * */


#define BUF_SIZE 100
#define EPOLL_SIZE 50
void setNoBlockingMode(int fd);
void error_handling(char *buf);

int main(int argc ,char* argv[]){

    int serv_sock,clnt_sock;
    struct sockaddr_in  serv_adr,clnt_adr;

    int str_len ,i;

    socklen_t adr_sz;
    char buf[BUF_SIZE];

    struct epoll_event *ep_events;
    struct epoll_event event;
    int epfd , event_cnt;

    if(argc != 2){
        printf("Usage : %s <port>\n",argv[0]);
        exit(1);    
    }

    // 建立服务器端socket
    serv_sock = socket(PF_INET,SOCK_STREAM,0);
    memset(&serv_adr,0,sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr)) == -1){
        error_handling("bind() error...");
    }
    if(listen(serv_sock,5) == -1){
        error_handling("listen() error ...");
    }

    epfd = epoll_create(EPOLL_SIZE);
    ep_events = malloc(sizeof(struct epoll_event)*EPOLL_SIZE);

    setNoBlockingMode(serv_sock);
    event.events = EPOLLIN;
    event.data.fd = serv_sock;
    epoll_ctl(epfd, EPOLL_CTL_ADD,serv_sock,&event);

    while(1){
        
        event_cnt = epoll_wait(epfd ,ep_events,EPOLL_SIZE,-1);
        if(event_cnt == -1){
            puts("epoll_wait() error");
            break;
        }

        puts("return epoll_wait()");

        for(i=0 ;i<event_cnt;i++){
            // 读到新的事件，增加到结构体数组中中
            if(ep_events[i].data.fd == serv_sock){
                adr_sz = sizeof(clnt_adr);
                clnt_sock = accept(serv_sock,(struct sockaddr*)&clnt_adr,&adr_sz);
                setNoBlockingMode(clnt_sock);
                // 边缘触发
                event.events = EPOLLIN | EPOLLET;
                event.data.fd = clnt_sock;
                epoll_ctl(epfd,EPOLL_CTL_ADD,clnt_sock,&event);
                printf("connected client: %d\n",clnt_sock);
            }else{

                // 这里添加while(1)循环：边缘触发方式中，发生事件时需要读取输入缓冲中的所有数据
                while(1){

                    str_len = read(ep_events[i].data.fd ,buf,BUF_SIZE);
                    if(str_len == 0){
                        epoll_ctl(epfd,EPOLL_CTL_DEL, ep_events[i].data.fd,NULL);
                        close(ep_events[i].data.fd);
                        printf("Close client: %d\n",ep_events[i].data.fd);
                        break;
                    }else if(str_len < 0){
                        // 意味着read()函数已经读取了输入缓冲中的全部数据
                        if(errno == EAGAIN){
                            break;
                        }
                    }else{
                        write(ep_events[i].data.fd,buf,str_len); // echo
                    }


                }





            }
        }


        
        
    }

    close(serv_sock);
    close(epfd);
    return 0;

}

void setNoBlockingMode(int fd){
    /* 将文件套接字改成非阻塞模式*/
    int flag = fcntl(fd,F_GETFL ,0); // 获得之前设置的属性信息
    fcntl(fd ,F_SETFL ,flag|O_NONBLOCK); // 添加非阻塞标志
}


void error_handling(char *msg){
    fputs(msg,stderr);
    fputc('\n',stderr);
    exit(1);
}



