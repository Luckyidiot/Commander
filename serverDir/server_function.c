#include "server_function.h"
#include <sys/ioctl.h>
#include <time.h>

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

void FileExe_naming(char* filename, int socketfd){
    /**
     * Each client will have an execution file
     * Those files will be named using their IP address and Date & Time to ensure the uniqueness
     * 
     * The IPaddr has the format of AAA.BBB.CCC.DDD at most so it needs 15 elements.
    */
    char IPaddr[15];
    time_t rawTime = time(NULL);
    struct sockaddr_in clientIP;
    socklen_t clientIP_size;
    
    memset(IPaddr, 0, sizeof(IPaddr));
    getpeername(socketfd, (struct sockaddr*) &clientIP, &clientIP_size);
    inet_ntop(AF_INET, &clientIP.sin_addr, IPaddr, 15);
    sprintf(filename, "fileExe/%s_%ld", IPaddr, rawTime);
    printf("Naming::::IP is %s\n", IPaddr);
    
}

void ChangeMode(const char* fileName){
    char shellCmd[52];
    sprintf(shellCmd, "chmod +x %s", fileName);
    system(shellCmd);
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
        while (FD_ISSET(*maxFD, readFDs) == false){
            *maxFD -= 1;
        }
    }

    printf("Handled successfully socket %d\n", fd);
}



void Receive_fileExe(int socketfd){
    size_t receivedBytes = 0;
    char bufferWriter[BANDWIDTH];
    int fileExe;
    struct sockaddr_in clientIP;
    socklen_t clientIP_size = sizeof(clientIP);
    char fileName[43];

    /**
     * Receiving the instruction from the clients and storing it in the file named by
     * the client's IP and date & time 
     * 
     * This function does not execute that instruction but simply put it into the fileExe folder
     * 
    */

    memset(fileName, 0, sizeof(fileName));
    FileExe_naming(fileName, socketfd);
    printf("fileName is %s\n", fileName);

    /**
     * This merely create the file but not do any I/O operation.
    */
    FILE* bufferFile;
    if ((bufferFile = fopen(fileName, "w")) == NULL){
        fprintf(stderr, "Fail to create instruction file %s\n", fileName);
        exit(EXIT_FAILURE);
    }
    fclose(bufferFile);


    /**
     * This open() open the file descriptor for I/O
    */
    if ((fileExe = open(fileName, O_WRONLY)) < 0){
        perror("Cannot open the instruction file to write to");
        exit(EXIT_FAILURE);
    }

    /**
     * Get data from file descriptor and read it to a local file
    */
    while ((receivedBytes = recv(socketfd, bufferWriter, BANDWIDTH, 0)) > 0){
        int writtenBytes;

        if ((writtenBytes = write(fileExe, bufferWriter, receivedBytes)) < 0){
            perror("ERROR: Writing into fileExe failed:");
            exit(EXIT_FAILURE);
        }
    }
    ChangeMode(fileName);

    close (fileExe);
}


void Write_Message(int socketfd, char* message){
    if (send(socketfd, message, (strlen(message) + 1), 0) < 0){
        perror("WRITE: ");
        exit(EXIT_FAILURE);
    }
}


