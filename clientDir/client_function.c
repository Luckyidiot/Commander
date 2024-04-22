
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

int IPv4_SocketCreate(){
    int socketfd = socket(PF_INET, SOCK_STREAM, 0);
    if (socketfd < 0){
        fprintf(stderr, "Socket creation failed");
        exit(EXIT_FAILURE);
    }
    return socketfd;
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

void Read_Message(int buffer_socketfd, char* recvMessage, size_t bandWidth){

    if (recv(buffer_socketfd, recvMessage, bandWidth, 0) < 0){
        perror("READ MESSAGE: ");
        exit(EXIT_FAILURE);
    }

}

void Signaling(int socketFD){
    /**
     * TASK: SENDING A SIGNAL TO EVERY DEVICES WITHIN LAN
    */
    int message = SIGNAL;
    
    if (send(socketFD, &message, sizeof(message), 0) < 0){
        perror("ERROR: Fail to signal");
        exit(EXIT_FAILURE);
    }
}

void Receive_respon(int socketFD){

    /*
    char* buffer[BANDWIDTH];
    volatile sig_atomic_t terminate = 0;
    void ScanningTermination(int signum){
        terminate = 1;
    }

    signal(SIGALRM, handler);
    alarm(10);

    
    while (recv(socketFD, buffer, BANDWIDTH, 0) > 0){

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
    */
}





