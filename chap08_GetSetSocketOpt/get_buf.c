#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>


// 查看IO缓冲大小
// SO_SNDBUF SO_RCVBUF: 读取当前IO缓冲大小，前者是输入缓冲大小，后者是输出缓冲大小。也可以用来更改

// setsockopt 只将我们希望的buffersize值传递，但是实际可能并不会按照我们想要的修改
void error_handling(char* message);

int main(int argc,char* argv[]){

    int sock;
    int snd_buf , rcv_buf ,state;
    socklen_t len;

    sock = socket(PF_INET,SOCK_STREAM,0);
    len = sizeof(snd_buf);

    // 获得输入缓冲大小
    state = getsockopt(sock , SOL_SOCKET,SO_SNDBUF ,(void *)&rcv_buf, &len);
    if(state){
        error_handling("getsocketopt() error.\r\n");
        exit(1);
    }
    printf("Input buffer size : %d\n",rcv_buf); // 16384

    state = getsockopt(sock , SOL_SOCKET,SO_SNDBUF ,(void *)&snd_buf, &len);
    if(state){
        error_handling("getsocketopt() error.\r\n");
        exit(1);
    }
    printf("Output buffer size : %d\n",snd_buf); // 16384

    /**更改缓冲区大小**/
    printf("\r\nChange bufsize \r\n\n");

    snd_buf = 1024*20;
    rcv_buf = 1024*20;

    state = setsockopt(sock , SOL_SOCKET,SO_SNDBUF ,(void *)&snd_buf, &len);
    if(state){
        error_handling("getsocketopt() error.\r\n");
        exit(1);
    }
    printf("We hope the buffer size : %d\n",snd_buf); // 这个结果不是最终设置的结果,是前面那个变量的值  20480

    state = getsockopt(sock , SOL_SOCKET,SO_SNDBUF ,(void *)&snd_buf, &len);
    if(state){
        error_handling("getsocketopt() error.\r\n");
        exit(1);
    }
    printf("the fact Output buffer size  : %d\n",snd_buf); // 40960

    return 0;

}

void error_handling(char* message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1) ;
}