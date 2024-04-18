
#include "clientDir/interface.h"


int main(int argc, char** argv){
    /**
     * Client will have to produce the binary file, and then send them to
     * the server.
     * 
     * 
     * ./client 127.0.0.1
    */

    int socketfd;
    struct sockaddr_in serverAddress;


    socketfd = IPv4_SocketCreate();
    serverAddress = Init_IPv4_addr(AF_INET, PORT, argv[1]);

    
    Connect_Server(socketfd, serverAddress);

    Send_file(socketfd, "message", ENABLE);

    close(socketfd);
    return 0;

}