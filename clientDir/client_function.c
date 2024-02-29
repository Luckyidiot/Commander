
#include "client_function.h"

void Send_fileExe(int socketfd, const char* fileName){

    /**
     * 1) fileExe opens ASSEMBLY file
     * 2) Calculate the fileSize
     * 3) Use sendfile() to send the content of file descriptor to the
     *    socket
     * 
    */
    int fileExe;
    off_t fileSize;

    if ((fileExe = open(fileName, O_RDONLY)) < 0){
        perror("Cannot open fileExe:");
        exit(EXIT_FAILURE);
    }

    /**
     * Move the file pointer to the end of the file, its location
     * at the end of the file will be the size of the file.
     * 
     * Have to rewind back to the beginning after getting the fileSize.
     * 
     * fileSize indicate the size of the file in BYTES
    */
    if ((fileSize = lseek(fileExe, 0, SEEK_END)) < 0){
        perror("Can not get the fileSize:");
        exit(EXIT_FAILURE);
    }
    if (lseek(fileExe, 0, SEEK_SET) < 0){
        perror("Can not rewind back to the beginning of the file:");
        exit(EXIT_FAILURE);
    }
    printf("The size of the file is %ld\n", fileSize);

    /**
     * Send the file to the server
    */
    ssize_t sentBytes;
    if ((sentBytes = sendfile(socketfd, fileExe, NULL, fileSize)) != fileSize){
        if (sentBytes < 0){
            perror("ERROR in sending file:");
            exit(EXIT_FAILURE);
        }
        else {
            fprintf(stderr, "ERROR: The number of sent bytes is not enough\n");
            exit(EXIT_FAILURE);
        }
    }
    printf("The entire file is sent successfully\n");
    /**
     * Close the file after finish sending
     * 
    */
    close(fileExe);
}

void Read_Message(int buffer_socketfd, char* recvMessage, size_t bandWidth){

    if (recv(buffer_socketfd, recvMessage, bandWidth, 0) < 0){
        perror("READ MESSAGE: ");
        exit(EXIT_FAILURE);
    }

}

void Connect_Server(int socketfd, struct sockaddr_in serverAddress){
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

int IPv4_SocketCreate(){
    int socketfd = socket(PF_INET, SOCK_STREAM, 0);
    if (socketfd < 0){
        fprintf(stderr, "Socket creation failed");
        exit(EXIT_FAILURE);
    }
    return socketfd;
}

