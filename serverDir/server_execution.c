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

void Cryptography(const char* filename_src, const char* filename_dst, uint8_t enabler, uint8_t mode, int key){
    /**
     * TASK: CRYPTOGRAPHY
     * 
     * 1) mode (2 options)    :: Select either Encryption or Decryption
     * M_ENCRYPT
     * M_DECRYPT
     * 
     * 2) enabler (2 options) :: disable or enable the cryptography - mainly used for testing and give more manipulation
     * ENBLE
     * DISABLE
     * 
     * 
     * ERROR: PASSING FILE DESCRIPTOR DOES NOT WORK
     * At first, I passed file descriptors for the src and dst and use them directly for the while() loop.
     * But the dst file is completely empty, which means it does not work, BUT THE REASONS ARE UNCLEAR.
     * SOLUTION: PASSING FILE NAME INSTEAD OF DESCRIPTOR
    */

    char buffer[BANDWIDTH];
    size_t readBytes;
    int srcFD;
    int dstFD;

    /**
     * TASK: OPEN THE FILES DEPENDING ON THEIR NAME
    */
    srcFD = File_CreateOpen(filename_src, M_OPEN);
    dstFD = File_CreateOpen(filename_dst, M_OPEN);

    /**
     * TASK: DISABLE THE KEY
     * 
     * The En/De-cryption still happens when it is disabled, but the output is the same
     * with the input. So nothing really changes.
    */
    if (!enabler){
        key = 0;
    }

    while ((readBytes = read(srcFD, buffer, BANDWIDTH)) > 0){
        
        if (mode){
            Encrypt(buffer, BANDWIDTH, key);
        }
        else {
            Decrypt(buffer, BANDWIDTH, key);
        }

        if (write(dstFD, buffer, readBytes) < 0){
            perror("ERROR: Server's cryptography fileExe failed:");
            exit(EXIT_FAILURE);
        }
        memset(buffer, 0, sizeof(buffer));
    }
    if (readBytes == -1){
        perror("ERROR: Fail to read the file for cryptography");
        exit(EXIT_FAILURE);
    }

    close(srcFD);
    close(dstFD);
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

void Change_AccessPermission(const char* filename, int mode){
    /**
     * TASK: CHANGE THE PERMISSION
     *
    */
    if (mode){
        //Executable
        if (chmod(filename, S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH) != 0){
            perror("File's permission is failed");
            exit(EXIT_FAILURE);
        }
    }
    else {
        //Regular
        if (chmod(filename, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH) != 0){
            perror("File's permission is failed");
            exit(EXIT_FAILURE);
        }
    }
}

int File_CreateOpen(const char* filename, int mode){
    /**
     * Two macros for mode:
     *      M_CREATE  1
     *      M_OPEN    0
    */
    
    int fileExe;
    
    if (mode){
        if ((fileExe = open(filename, O_RDWR | O_CREAT | O_TRUNC)) < 0){
            perror("Cannot create fileExe:");
            exit(EXIT_FAILURE);
        }
    }
    else {
        if ((fileExe = open(filename, O_RDWR)) < 0){
            perror("Cannot open fileExe:");
            exit(EXIT_FAILURE);
        }
    }

    return fileExe;
}

