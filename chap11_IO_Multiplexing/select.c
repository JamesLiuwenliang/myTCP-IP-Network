// select()函数可以将多个文件描述符集中到一起统一监听
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>
#define BUF_SIZE 30

int main(int argc,char *argv[]){

    // 数组变量，操作这个变量通过一些宏定义
    fd_set reads ,temps;
    int result ,str_len;
    char buf[BUF_SIZE];

    // 用来设置超时的结构体 
    struct timeval timeout;

    FD_ZERO(&reads);
    // 将文件描述符0对应的位设置为1，0位就是标准输入位，即需要监视标准输入位的变化
    FD_SET(0,&reads); 

    /*
    // 必须要每次都设置，不能提前设置
    timeout.tv_sec = 5;
    timeout.tv_usec = 5000;
    */

    while(1){

        // 必须有一个转存的fd_set, 因为调用完select函数之后所有位都初始化为0，
        // 为了记住初始值，必须采用这种方法，这也是通用方法
        temps = reads;

        // 运行后5s内没有任何输入，则发生超时
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;

        // 调用select()，如果有控制台输入数据，则返回大于0的整数
        result = select(1, &temps,0,0,&timeout);

        if(result == -1){
            puts("Select() error...");
            break;
        }else if(result == 0){
            puts("Time is over...");
        }else{

            // select()监听到变化，验证发生变化的文件描述符是否为标准输入，
            // 如果是，则从标准输入读取数据并向控制台输出
            if(FD_ISSET(0,&temps)){
                str_len = read(0,buf,BUF_SIZE);
                buf[str_len] = 0;
                printf("message from console: %s\n",buf);
            }



        }

    }

    return 0;

}