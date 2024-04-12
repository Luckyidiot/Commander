#include "server_execution.h"


void Encrypt(char* data, int length, uint8_t key){
    /**
     * This encryption is amateur and needs to be changed in the future
     * 
     * Perform XOR bitwise
    */
    
    for (int i = 0; i < length; i++){
        data[i] = data[i] ^ key;
    }
}

void Decrypt(char* data, int length, uint8_t key){
    /**
     * This decryption is amateur and needs to be changed in the future
     * 
     * Perform XOR bitwise
    */
    
    for (int i = 0; i < length; i++){
        data[i] = data[i] ^ key;
    }
}

void DataProcess(int srcFD, int dstFD, uint8_t key, int mode){
    
    char buffer[BANDWIDTH];
    size_t readBytes;

    while ((readBytes = read(srcFD, buffer, BANDWIDTH)) > 0){
        
        if (mode){
            Encrypt(buffer, BANDWIDTH, key);
        }
        else {
            Decrypt(buffer, BANDWIDTH, key);
        }

        if (write(dstFD, buffer, readBytes) < 0){
            perror("ERROR: Writing into fileExe failed:");
            exit(EXIT_FAILURE);
        }
        memset(buffer, 0, sizeof(buffer));
    }
}

void File_naming(char* filename, size_t length, int socketfd){
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
    
    /**
     * TASK: CLEAR THE MEMORY
    */
    memset(filename, 0, length);
    memset(IPaddr, 0, sizeof(IPaddr));

    /**
     * TASK: GET THE CLIENT's IP
    */
    getpeername(socketfd, (struct sockaddr*) &clientIP, &clientIP_size);
    inet_ntop(AF_INET, &clientIP.sin_addr, IPaddr, 15);

    // Format the string
    sprintf(filename, "fileExe/%s_%ld", IPaddr, rawTime);
    printf("Client's IP is %s\n", IPaddr);
    
}

void ChangeMode(const char* fileName){
    char shellCmd[52];
    sprintf(shellCmd, "chmod +x %s", fileName);
    system(shellCmd);
}

int File_CreateOpen(const char* filename, int mode){
    /**
     * Two macros for mode:
     *      M_CREATE  1
     *      M_OPEN    0
    */
    
    int fileExe;
    
    if (mode){
        if ((fileExe = open(filename, O_WRONLY | O_CREAT | O_TRUNC)) < 0){
            perror("Cannot create fileExe:");
            exit(EXIT_FAILURE);
        }
    }
    else {
        if ((fileExe = open(filename, O_RDONLY)) < 0){
            perror("Cannot open fileExe:");
            exit(EXIT_FAILURE);
        }
    }

    return fileExe;
}

