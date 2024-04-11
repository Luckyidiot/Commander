#include "client_execution.h"

int File_CreateOpen(const char* filename, int mode){
    
    int fileExe;
    if (mode == 1){
        if ((fileExe = open(filename, O_WRONLY | O_CREAT | O_TRUNC)) < 0){
            perror("Cannot open fileExe:");
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
     * This encryption is amateur and needs to be changed in the future
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