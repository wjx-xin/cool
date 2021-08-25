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
// #include "httpResponse.hpp"
#include<string>
using namespace std;

void handle2(int conn){
    std::cout << "conn接收" << conn << std::endl;
    char buf[1024];
    string data = "";
    memset(buf,0,sizeof(buf));
    int len = 1;
    int sum = 0;
    int old = fcntl(conn,F_GETFL,0);
    int newf = old | O_NONBLOCK;
    fcntl(conn,F_SETFL,newf);
    while(len != -1){
        len = recv(conn,buf,sizeof(buf),0);
        cout << len << endl;
        // if (len == 0) {
        //     break;
        // }
        sum += len;
        data += string(buf);
        
    }
    if(strcmp(buf,"exit") == 0) {
        close(conn);
    }
    httpResponse* httpR = new httpResponse(200,"OK","text/plain","hello, network programming");
    char* sendBuf = httpR->makePacket();
    send(conn,sendBuf,sizeof(buf),0);

    memset(buf,0,sizeof(buf));
    cout << " --- " <<data << endl;
}
int main() {
    // socket和epoll都封装得比较简单
    socker s("127.0.0.1",8888);
    int server_sock = s.getSock();

    epoll epoller(server_sock,100);
    int epfd = epoller.getEpfd();
    struct epoll_event* events = epoller.getEvents();

    ThreadPool pool(2);
    while(true) {
        int res = epoller.wait();
        for(int i = 0;i < res;i++){
            if(events[i].data.fd == server_sock) {
                Acceptor(server_sock,epfd);
            } else {
                ThreadTask task(handle2,events[i].data.fd);
                pool.append(task);   
            }
        }
    }
    
    close(server_sock);
    return 0;
}