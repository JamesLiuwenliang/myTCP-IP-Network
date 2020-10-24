#include <stdio.h>
#include <sys/uio.h>

#define BUF_SIZE 100

int main(int argc ,char *argv[]){

    /*  struct ivoec{
     *      void* iov_base; // 缓冲地址
     *      size_t iov_len; // 缓冲大小
     *  }
     */
    struct iovec vec[2];
    char buf1[BUF_SIZE] = {0,};
    char buf2[BUF_SIZE] = {0,};
    int str_len ;

    vec[0].iov_base = buf1;
    vec[0].iov_len = 5;
    vec[1].iov_base = buf2;
    vec[1].iov_len = BUF_SIZE;

    // 缓冲区的数据一并发送，会发送 "ABC" 和 "1234"
    str_len = readv(0,vec,2);
    
    printf("Read bytes: %d \n",str_len);
    printf("First message : %s\n",buf1);
    printf("Second message: %s\n",buf2);
    return 0;
}
