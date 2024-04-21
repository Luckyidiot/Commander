
#include "clientDir/interface.h"


int main(int argc, char** argv){
    /**
     * Client will have to produce the binary file, and then send them to
     * the server.
     * 
     * 
     * ./client 127.0.0.1
    */

    int socketFD;
    struct sockaddr_in serverAddress;


    socketFD = IPv4_SocketCreate();
    serverAddress = Init_IPv4_addr(AF_INET, PORT, argv[1]);

    
    Establish_connection(socketFD, serverAddress);

    Send_file(socketFD, "message", ENABLE);

    close(socketFD);
    return 0;

}