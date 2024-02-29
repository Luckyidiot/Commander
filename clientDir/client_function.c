
#include "client_function.h"

void Send_fileExe(int socketfd, const char* fileName){

    /**
     * 1) fileExe opens ASSEMBLY file
     * 2) Calculate the fileSize
     * 3) Read and Send constantly to the server until sent bytes are enough
     * 
    */
    int fileExe;
    char bufferReader[BANDWIDTH];
    size_t fileSize;

    if ((fileExe = open(fileName, O_RDONLY)) < 0){
        perror("Cannot open fileExe:");
        exit(EXIT_FAILURE);
    }

    /**
     * Move the file pointer to the end of the binary file, its location
     * at the end of the file will be the size of the file.
     * 
     * The first fseek() move the pointer to the end of the file
     * Thhe last one rewind it to the begining.
     * 
     * fileSize indicate the size of the file in BYTES
    */
    fseek(fileExe, 0, SEEK_END);
    fileSize = ftell(fileExe);
    fseek(fileExe, 0, SEEK_SET);

    /**
     * Read the file to bufferReader and send it
     * 
     * This protocol is still still unreliable cannot be used in practice, but the idea is correct and
     * still work in testing scope.
     * 
     * It needs the following:
     *  # Confirm that the total sent bytes are equal to the size of the file
     *  # Confirm that in case fread() reads successfully, it reads [BANDWIDTH] bytes and the remainding bytes
     *    in the last turn.
     *  # Confirm that in the last turn of send, when send() sent successfully, it sent the exact number of
     *    remainding bytes before exit the loop.
    */
    
    int readBytes;
    int sentBytes;

    if ((readBytes = fread(bufferReader, 1, BANDWIDTH, fileExe)) < 0){
        //Shall the fread not read enough all the file, it will return error.
        fprintf(stderr, "Execution file received is incorrect, only %d was read\n while we have %ld\n", readBytes, fileSize);
        exit(EXIT_FAILURE);
    }

    if ((sentBytes = send(socketfd, bufferReader, BANDWIDTH, 0)) >= 0) {
        if (sentBytes < BANDWIDTH){
            
        }
    }
    else {
        perror("Sending ERROR:");
        exit(EXIT_FAILURE);
    }


    /**
     * Close the file after finish sending
     * 
    */
    fclose(fileExe);
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

