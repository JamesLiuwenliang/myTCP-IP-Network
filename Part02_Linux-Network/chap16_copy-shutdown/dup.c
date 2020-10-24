#include <stdio.h>
#include <unistd.h>

/*
    目标：利用标准IO实现半关闭
    “销毁所有文件描述符后才能销毁套接字”
    所以可以在套接字下创建两套文件描述符。分别针对读、写缓冲区。
    这里的文件描述符的复制(dup()函数) ：为了访问同一文件或套接字，创建另一个文件描述符
*/
int main(int argc,char *argv[]){
    int cfd1,cfd2;
    char str1[] = "Hello\n";
    char str2[] = "World\n";

    /* dup2和dup1的区别就是是否明确指定文件描述符整数值
    *  int dup(int fildes);
    *  int dup2(int fildes,int fildes2);
    *  fildes:需要复制的文件描述符,fildes2:明确指定一个整数值
    */
    cfd1 = dup(1); // 文件描述符1就是标准输出,则cfd1也可以表示标准输出
    cfd2 = dup2(cfd1,7); // 指定cfd2为7

    printf("fd1=%d,fd2=%d \n",cfd1,cfd2);
    write(cfd1,str1,sizeof(str1));
    write(cfd2,str2,sizeof(str2));

    close(cfd1);
    close(1);

    write(cfd2,str2,sizeof(str2));
    // 依然可以输出结果,文件描述符依然是标准输出
    close(cfd2);
    // 关闭=后,就不会输出结果
    write(cfd2,str2,sizeof(str2));
    return 0;
}





