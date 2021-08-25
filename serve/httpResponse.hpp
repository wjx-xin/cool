#include<string>
#include<cstring>
class httpResponse{
private:
    int     statusCode;
    char*   statusMsg;
    char*   contentType;
    char*   body;
    char*   packet;
    int     size;  // to new the response packet memory
public:
    httpResponse();
    httpResponse(int statusCode,const char* statusMsg,const char* contentType,const char* body);
    char* makePacket();
    ~httpResponse();
};


httpResponse::httpResponse(){}
httpResponse::httpResponse(int statusCode,const char* statusMsg,const char* contentType,const char* body){
    this->statusCode = statusCode;

    this->statusMsg = new char[strlen(statusMsg)];
    this->contentType = new char [strlen(contentType)];
    this->body = new char [strlen(body)];
    strcpy(this->statusMsg,statusMsg);
    strcpy(this->contentType,contentType);
    strcpy(this->body,body);
}
httpResponse::~httpResponse(){
    delete  []statusMsg;
    delete  []contentType;
    delete  []body;
    delete  []packet;
}
char* httpResponse::makePacket(){
    size = 1024;
    packet = new char[size];

    char buf[32]={'\0'};
    snprintf(buf, sizeof(buf), "HTTP/1.1 %d ", 200);
    packet = strcat(packet,buf);
    packet = strcat(packet,statusMsg);
    packet = strcat(packet,"\r\n");
    snprintf(buf, sizeof(buf), "Content-Length: %zd\r\n", strlen(body));
    packet = strcat(packet,buf);
    packet = strcat(packet,"Connection: Keep-Alive\r\n");

    packet = strcat(packet,"\r\n");
    packet = strcat(packet,body);

    return packet;
}
// int main(){
//     httpResponse* httpR = new httpResponse(200,"OK","text/plain","hello, network programming");
//     // std::cout << httpR->makePacket() << std::endl;
//     printf("%s\n",httpR->makePacket());
//     delete httpR;
//     return 0;
// }