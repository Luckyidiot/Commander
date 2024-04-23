#include "server_function.h"
#include <sys/ioctl.h>
#include <time.h>


void Socket_modify(const int en, int socketFD, int option){

    if (setsockopt(socketFD, SOL_SOCKET, option, &en, sizeof(en)) < 0){
        perror("ERROR in Setting socket");
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

void BindAddr(int socketfd, struct sockaddr_in addr){
    if (bind(socketfd, (struct sockaddr*) &addr, sizeof(addr)) < 0){
        perror("BIND:");
        exit(EXIT_FAILURE);
    }

}

int IPv4_SocketCreate(int proctocol){
    int socketfd = socket(PF_INET, proctocol, 0);
    if (socketfd < 0){
        perror("ERROR in SOCKET CREATION");
        exit(EXIT_FAILURE);
    }
    return socketfd;
}

void ListenEvent(int socketfd, int queueLength){
    //printf("Listening...\n");
    if (listen(socketfd, queueLength) < 0){
        perror("LISTEN: ");
        exit(EXIT_FAILURE);
    }

    printf("LISTENING........\n");
}

int AcceptConnection(int socketfd){

    /**
     * 
     * This function accepts the connection request from the client
     * 
     * clientIP is now temporarily not used, but its aim is to consider if the connection 
     * is reliable, othewise the connection will be suspended.
    */

    struct sockaddr_in clientIP;
    socklen_t clientIP_size = sizeof(clientIP);

    int buffer_socketfd = accept(socketfd, (struct sockaddr*) &clientIP, &clientIP_size);
    if (buffer_socketfd < 0){
        perror("ACCEPT: ");
        exit(EXIT_FAILURE);
    }

    return buffer_socketfd;

}

void Closing_procedure(int fd, int* maxFD, fd_set* readFDs){
    /**
     * Close the file descriptor
     * 
     * Changing the index to recognize the 
    */
    
    close(fd);
    FD_CLR(fd, readFDs);
    if (fd == *maxFD){
        while (FD_ISSET(*maxFD, readFDs) == 0){
            *maxFD -= 1;
        }
    }

    printf("Handled successfully socket %d\n", fd);
}

void Receive_attempt(int socketFD, int fileDST){

    ssize_t receivedBytes = 0;
    char buffer[BANDWIDTH];

    while ((receivedBytes = recv(socketFD, buffer, BANDWIDTH, 0)) > 0){

        if (write(fileDST, buffer, receivedBytes) < 0){
            perror("ERROR: Writing into fileExe failed:");
            exit(EXIT_FAILURE);
        }
        memset(buffer, 0, BANDWIDTH);
    }

}

void Write_Message(int socketfd, char* message){
    if (send(socketfd, message, (strlen(message) + 1), 0) < 0){
        perror("WRITE: ");
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