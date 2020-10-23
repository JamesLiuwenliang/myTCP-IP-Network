#include <stdio.h>
#include <sys/uio.h>

int main(int argc ,char *argv[]){

    /*  struct ivoec{
     *      void* iov_base; // 缓冲地址
     *      size_t iov_len; // 缓冲大小
     *  }
     */
    struct iovec vec[2];
    char buf1[] = "ABCDEF";
    char buf2[] = "1234567";
    int str_len ;

    vec[0].iov_base = buf1;
    vec[0].iov_len = 3;
    vec[1].iov_base = buf2;
    vec[1].iov_len = 4;

    // 缓冲区的数据一并发送，会发送 "ABC" 和 "1234"
    str_len = writev(1,vec,2);
    puts("");
    printf("Write bytes: %d \n",str_len);
    return 0;
}
