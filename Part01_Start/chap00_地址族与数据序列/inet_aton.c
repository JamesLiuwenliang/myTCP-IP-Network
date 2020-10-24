#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

// 两个函数的作用相同，但是传的参数不同
// int inet_aton(const char* string,struct in_addr *addr); // 成功时返回1，失败时返回0
int main(int argc,char *argv[]){
    char *addr = "127.232.124.79";

    struct sockaddr_in addr_inet;

    if(!inet_aton(addr,&addr_inet.sin_addr)){
        printf("Error occured!\n");
    }else{
        printf("Network ordered integer addr : %#lx \n",addr_inet.sin_addr.s_addr);
    }


    return 0;

}
