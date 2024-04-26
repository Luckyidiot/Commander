
#include "client_function.h"
#include <signal.h>


void Establish_connection(int socketfd, struct sockaddr_in serverAddress){
    if (connect(socketfd, (struct sockaddr*) &serverAddress, sizeof(serverAddress)) < 0){
        perror("CONNECT: ");
        exit(EXIT_FAILURE);
    }
}

struct sockaddr_in Init_IPv4_addr(short sin_family, int port, char* address){

    struct sockaddr_in addr;

    memset(&addr, 0, sizeof(addr));

    addr.sin_family = sin_family;
    addr.sin_port = htons(port);

    
    if (strcmp(address, "INADDR_LOOPBACK") == 0){
        addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    }
    else if (strcmp(address, "INADDR_ANY") == 0){
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
    }
    else if (strcmp(address, "INADDR_BROADCAST") == 0){
        addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
    }
    else if (strcmp(address, "INADDR_NONE") == 0){
        addr.sin_addr.s_addr = htonl(INADDR_NONE);
    }
    else {
        if (inet_pton(AF_INET, address, &addr.sin_addr) < 0){
            printf("inet_pton() error for %s\n", address);
            exit(EXIT_FAILURE);
        }
    }
    
    return addr;
}

int IPv4_SocketCreate(int proctocol){
    int socketfd = socket(PF_INET, proctocol, 0);
    if (socketfd < 0){
        fprintf(stderr, "Socket creation failed");
        exit(EXIT_FAILURE);
    }
    return socketfd;
}

void Socket_modify(const int en, int socketFD, int option){

    if (setsockopt(socketFD, SOL_SOCKET, option, &en, sizeof(en)) < 0){
        perror("Set socket opt: ");
        exit(EXIT_FAILURE);
    }
}

void SendAttempt_files(int socketFD, int fileFD, off_t fileSize, const char* filename){
    

    ssize_t sentBytes;
    if ((sentBytes = sendfile(socketFD, fileFD, NULL, fileSize)) != fileSize){
        if (sentBytes < 0){
            perror("ERROR in sending file:");
            exit(EXIT_FAILURE);
        }
        else {
            fprintf(stderr, "ERROR: The number of sent bytes is not enough\n");
            exit(EXIT_FAILURE);
        }
    }
    printf("The entire file %s is sent successfully\n", filename);
}

void SendAttempt_any(int socketFD, int fileFD, const char* name){
    
    ssize_t readBytes = 0;
    char* buffer[BANDWIDTH];

    while ((readBytes = read(fileFD, buffer, BANDWIDTH)) > 0){

        if (send(socketFD, buffer, readBytes, 0) < 0){
            perror("ERROR: Client's cryptography failed");
            exit(EXIT_FAILURE);
        }
        memset(buffer, 0, sizeof(buffer));
    }
    if (readBytes == -1){
        perror("ERROR in SendAttempt_any(): Fail to read to be sent");
        exit(EXIT_FAILURE);
    }

    printf("File %s was sent successfully\n", name);

}

void SendAttempt_message(int protocol, int socketFD, const char* message, struct sockaddr_in* IP_dst){
    /**
     * TASK: SENDING message
     * 
     * Depending on the protocol, the function will use different sending mechanics
    */

    if (protocol == TCP){
        if (send(socketFD, message, (strlen(message) + 1), 0) < 0){
            perror("ERROR TCP:");
            exit(EXIT_FAILURE);
        }
    }
    else if (protocol == UDP){
        if (sendto(socketFD, message, (strlen(message) + 1), 0, (struct sockaddr*) IP_dst, sizeof(struct sockaddr)) < 0){
            perror("ERROR UDP:");
            exit(EXIT_FAILURE);
        }
    }
    else {
        fprintf(stderr, "Specified protocol is not supported for SENDING\n");
        exit(EXIT_FAILURE);
    }
    
}

void ReceiveAttempt_message(int protocol, int socketFD, char* recvMessage, size_t bandWidth, struct sockaddr_in* IP_src){
    /**
     * TASK: RECEIVING message
     * 
     * Depending on the protocol, the function will use different receiving mechanics
    */

    if (protocol == TCP){
        if (recv(socketFD, recvMessage, bandWidth, 0) < 0){
            perror("READ MESSAGE: ");
            exit(EXIT_FAILURE);
        }
    }
    else if (protocol == UDP) {
        socklen_t addrSize = sizeof(struct sockaddr);
        if (recvfrom(socketFD, recvMessage, bandWidth, 0, (struct sockaddr*) IP_src, &addrSize) < 0){
            perror("ERROR UDP:");
            exit(EXIT_FAILURE);
        }
    }
    else {
        fprintf(stderr, "Specified protocol is not supported for RECEIVING\n");
        exit(EXIT_FAILURE);
    }

}





