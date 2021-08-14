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
class socker{
    private:
        int sock;
        struct sockaddr_in server_addr;
    public:
        socker(const char* ip,int port){
            sock = socket(PF_INET,SOCK_STREAM,0);
            server_addr.sin_family = AF_INET;
            server_addr.sin_port = htons(port);
            server_addr.sin_addr.s_addr = inet_addr(ip);
            // 绑定套接字（文件）和主机地址
            if(bind(sock,(struct sockaddr*)&server_addr,sizeof(server_addr)) == -1){
                perror("bind");
                exit(1);
            }
            if(listen(sock,20) == -1) {
                perror("listen");
                exit(1);
            }
        }
        socker(){};
        int getSock(){
            return sock;
        }
};
class epoll{
private:
    int epfd;
    // int size = 0;
    struct epoll_event* events;
public:
    epoll(int server_sock,int size,int flag = 0){
        events = new struct epoll_event[size];
        epfd = epoll_create(size);

        struct epoll_event event;
        event.events = EPOLLIN;
        event.data.fd = server_sock;
        epoll_ctl(epfd,EPOLL_CTL_ADD,server_sock,&event);

    }
    int getEpfd(){
        return epfd;
    }
    int wait(int timeout = -1){
        int res = epoll_wait(epfd,events,1,timeout);
        return res;
    }
    void add(int flag,int fd) {
        struct epoll_event event;
        event.events = flag;
        event.data.fd = fd;
        epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&event);
    }
    struct epoll_event* getEvents(){
        return events;
    }
};