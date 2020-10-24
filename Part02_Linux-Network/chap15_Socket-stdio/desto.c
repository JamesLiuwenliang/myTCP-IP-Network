#include <stdio.h>
#include <fcntl.h>
/*
    fopen 相较于 open，以及fgets/fputs相较于read/write函数，作为标准输入/输出，因为建立的缓存，一般传输大文件的速度会快速很多
    因为使用方法一般是一次从缓存拿多次的数据，然后封装传输；但是read等方法是拿一次发一次，会同时包含很多冗余信息
    
*/
int main(void){
    FILE *fp;
    // 使用open创建文件，并返回文件描述符
    int fd = open("data.dat",O_WRONLY|O_CREAT|O_TRUNC);

    if(fd == -1){
        fputs("file open error",stdout);
        return -1;
    }

    // 将文件描述符转换为FILE指针,因为第二个参数的原因,因此返回写模式的FILE指针
    fp = fdopen(fd,"w");
    fputs("Network C programming\n",fp);

    // 调用后无需关闭文件描述符
    fclose(fp);

    return 0;


}
