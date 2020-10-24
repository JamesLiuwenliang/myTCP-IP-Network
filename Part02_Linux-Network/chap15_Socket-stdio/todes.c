#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

int main(void){
    FILE *fp;
    int fd = open("data.dat",O_WRONLY|O_CREAT | O_TRUNC);

    if(fd == -1){
        fputs("open error..",stdout);
        exit(1);    
    }

    printf("First file descriptor : %d\n",fd);
    // 将文件描述符转换为FILE指针
    fp = fdopen(fd,"w");
    fputs("Network C programming\n",fp);

    // 调用fileno函数再次转回文件描述符 
    printf("Second file descriptor: %d\n",fileno(fp));
    fclose(fp);
    return 0;    

}

