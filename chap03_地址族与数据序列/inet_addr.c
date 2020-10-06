#include <stdio.h>
#include <arpa/inet.h>

int main(int argc,char *argv[]){
    char *addr1 = "1.2.3.4";
    char *addr2 = "1.2.3.255";

    unsigned long conv_addr = inet_addr(addr1);
    if(conv_addr==INADDR_NONE){
        printf("Error occured!\n");
    }else{
        printf("Network ordered integer addr : %#lx \n",conv_addr);
    }

    // 如果IP传来的是 1.2.3.256 ，最后输出的应该是失败的
    conv_addr = inet_addr(addr2);
    if(conv_addr==INADDR_NONE){
        printf("Error occured!\n");
    }else{
        printf("Network ordered integer addr : %#lx \n",conv_addr);
    }

    return 0;

}
