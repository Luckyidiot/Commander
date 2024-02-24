#include "server_function.h"


void SetSocket_REUSE(int sockfd, int level, int optname, const void* optval, socklen_t optlen){

    if (setsockopt(sockfd, level, optname, optval, optlen) < 0){
        perror("Set socket opt: ");
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

int Create_IPv4Server(short sin_family, int port, char* address, int level, int optname, const void* optval, socklen_t optlen){
    int socketfd = socket(PF_INET, SOCK_STREAM, 0);
    if (socketfd < 0){
        printf("SOCKET ERROR");
        exit(EXIT_FAILURE);
    }
    /**
     * struct sockaddr_in serverAddress = Init_IPv4_addr(AF_INET, PORT, "INADDR_ANY");
    */
    
    struct sockaddr_in serverAddress = Init_IPv4_addr(sin_family, port, address);
    BindAddr(socketfd, serverAddress);

    SetSocket_REUSE(socketfd, level, optname, optval, optlen);

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

int AcceptConnection(struct sockaddr_in* clientAddress, socklen_t clientAddressSize, int socketfd){

    /**
     * Because accept() requires a pointer to the size of cliendAddress so we need to take that
     * as a parameter.
     * 
     * Implicitly understand that clientAddressSize = sizeof(clientAddress)
    */

    int buffer_socketfd = accept(socketfd, (struct sockaddr*) clientAddress, &clientAddressSize);
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
        while (FD_ISSET(*maxFD, readFDs) == false){
            *maxFD -= 1;
        }
    }
}

void Receive_fileExe(int socketfd, const char* fileName){
    size_t readBytes;
    char bufferWriter[BANDWIDTH];
    off_t fileSize;
    FILE* fileExe;

    if ((fileExe = fopen(fileName, "wb")) == NULL){
        fprintf(stderr, "Cannot open the instruction file to write to\n");
        exit(EXIT_FAILURE);
    }

    /**
     * Get the data sent by the client into the bufferWriter
    */
    if ((readBytes = recv(socketfd, bufferWriter, 159, 0)) >= 0){
        printf("%ld was read\n", readBytes);
        if (readBytes != 9){
            fprintf(stderr, "ERROR: Received bytes are not enough\n");
            exit(EXIT_FAILURE);
        }
    }
    else {
        // recv() has a fatal error
        perror("READ MESSAGE: ");
        exit(EXIT_FAILURE);
    }
    
    int a;
    if ((a = fwrite(bufferWriter, 1, readBytes, fileExe)) != 9){
        fprintf(stderr, "Execution file received is incorrect\n");
        exit(EXIT_FAILURE);
    }

    fclose (fileExe);

    printf("Written bytes is %d\n", a);
    printf("Check the newly created file\n");
    FILE* trialExe = fopen("trial_text", "r");
    if (trialExe == NULL){
        fprintf(stderr, "Cannot open the instruction file to read to\n");
        exit(EXIT_FAILURE);
    }
    fseek(trialExe, 0, SEEK_END);
    fileSize = ftell(trialExe);
    printf("The size of the newly created file is: %ld\n", fileSize);

    fclose(trialExe);

}

void Write_Message(int socketfd, char* message){
    if (send(socketfd, message, (strlen(message) + 1), 0) < 0){
        perror("WRITE: ");
        exit(EXIT_FAILURE);
    }
}


