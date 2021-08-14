#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<sys/shm.h>
#include<iostream>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
// #include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <iostream>
#include <netdb.h>
#define BUFSIZE 1024
int main() {
    // std::cout << inet_ntop((gethostbyname("www.baidu.com"))->h_addr_list) << std::endl;
    int client_sock = socket(PF_INET,SOCK_STREAM,0);
    struct sockaddr_in client_addr;
    client_addr.sin_family = PF_INET;
    client_addr.sin_port = htons(8888);
    client_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if(connect(client_sock,(struct sockaddr *)&client_addr,sizeof(client_addr)) == -1) {
        perror("bind");
        exit(1);
    }
    char sendbuf[BUFSIZE];
    char recvbuf[BUFSIZE];
    while(true) {
        memset(recvbuf,0,sizeof(recvbuf));
        memset(sendbuf,0,sizeof(sendbuf));
        std::cin >> sendbuf;
        // recv(client_sock,recvbuf,sizeof(recvbuf),0);
        // std::cout << recvbuf << std::endl;
        send(client_sock,sendbuf,sizeof(sendbuf),0);
        if(strcmp("exit",sendbuf) == 0) {
            break;
        }

    }
    close(client_sock);
    return 0;
}