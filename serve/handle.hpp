#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <iostream>
#include<sys/epoll.h>
#include "httpResponse.hpp"
void Acceptor(int server_sock,int epfd){
    struct epoll_event event; // 此处考虑改为堆上申请
    struct sockaddr_in client_addr;
    socklen_t len = sizeof(client_addr);
    int conn = accept(server_sock,(struct sockaddr*)&client_addr,&len);
    std::cout << "连接------" << conn << std::endl;
    event.data.fd = conn;
    event.events = EPOLLIN | EPOLLET;  // ET模式
    epoll_ctl(epfd,EPOLL_CTL_ADD,conn,&event);
}
void handle(int conn){
    std::cout << "conn接收" << conn << std::endl;
    char buf[1024];
    memset(buf,0,sizeof(buf));
    int len = recv(conn,buf,sizeof(buf),0);
    if(strcmp(buf,"exit") == 0) {
        close(conn);
    }
    httpResponse* httpR = new httpResponse(200,"OK","text/plain","hello, network programming");
    char* sendBuf = httpR->makePacket();
    send(conn,sendBuf,sizeof(buf),0);
    // int r = send(conn,buf,sizeof(buf),0);
    std::cout << buf << "长度" << strlen(buf) <<"\n";
}