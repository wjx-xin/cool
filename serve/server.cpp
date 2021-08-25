#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<iostream>
#include"pool.hpp"
#include"sock.hpp"
#include "handle.hpp"
#include<signal.h>
using namespace std;

int main() {
    // socket和epoll都封装得比较简单
    socker s("127.0.0.1",8888);
    int server_sock = s.getSock();

    epoll epoller(server_sock,20);
    int epfd = epoller.getEpfd();
    struct epoll_event* events = epoller.getEvents();

    ThreadPool pool(2);

    while(true) {
        int res = epoller.wait();
        for(int i = 0;i < res;i++){
            if(events[i].data.fd == server_sock) {
                Acceptor(server_sock,epfd);
            } else {
                ThreadTask task(handle,events[i].data.fd);
                pool.append(task);   
            }
        }
    }
    
    close(server_sock);
    return 0;
}