#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>

/* select():每次监听到套接字变化，都会向操作系统传递监视对象的信息
 * epoll():仅向操作系统传递一次监视对象的信息，监视范围和内容发生变化的时候，只通知发生变化（只有Linux支持）
 *
 * 
 * */


#define BUF_SIZE 100
#define EPOLL_SIZE 50
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

    // int epoll_create(int size); // 这个size是推荐大小,但Linux不会参考,会动态分配的。
    // 返回epoll文件描述符,也需要close()掉
    epfd = epoll_create(EPOLL_SIZE);

    // 这个结构体需要malloc重新分配内存
    ep_events = malloc(sizeof(struct epoll_event)*EPOLL_SIZE);

    // 发生需要读取数据的情况(事件)时
    event.events = EPOLLIN;
    event.data.fd = serv_sock;

    // int epoll_ctl(int epfd,    int op,int fd,struct epoll_event *event);
    //       epoll的文件描述符;添加删除更改操作;需要注册的监视对象文件描述符;监视对象的事件类型
    // epfd例程中注册serv_sock文件描述符，主要目的是监视event中的事件(即需要从客户端读取数据的情况)
    epoll_ctl(epfd, EPOLL_CTL_ADD,serv_sock,&event);

    while(1){
        
        // int epoll_wait(int epfd,struct epoll_event* events,int maxevents,int timeout);
        //返回发生事件的文件描述符数;                   可以保存的最大事件数;等待时间，传递-1为一直等待直到事件发生
        event_cnt = epoll_wait(epfd ,ep_events,EPOLL_SIZE,-1);
        if(event_cnt == -1){
            puts("epoll_wait() error");
            break;
        }

        for(i=0 ;i<event_cnt;i++){
            // 读到新的事件，增加到结构体数组中中
            if(ep_events[i].data.fd == serv_sock){
                adr_sz = sizeof(clnt_adr);
                clnt_sock = accept(serv_sock,(struct sockaddr*)&clnt_adr,&adr_sz);
                event.events = EPOLLIN;
                event.data.fd = clnt_sock;
                epoll_ctl(epfd,EPOLL_CTL_ADD,clnt_sock,&event);
                printf("connected client: %d\n",clnt_sock);
            }else{


                str_len = read(ep_events[i].data.fd ,buf,BUF_SIZE);
                if(str_len == 0){
                    epoll_ctl(epfd,EPOLL_CTL_DEL, ep_events[i].data.fd,NULL);
                    close(ep_events[i].data.fd);
                    printf("Close client: %d\n",ep_events[i].data.fd);
                }else{
                    write(ep_events[i].data.fd,buf,str_len); // echo
                }


            }
        }


        
        
    }

    close(serv_sock);
    close(epfd);
    return 0;

}


void error_handling(char *msg){
    fputs(msg,stderr);
    fputc('\n',stderr);
    exit(1);
}



