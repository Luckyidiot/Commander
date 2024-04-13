#include "client_execution.h"

off_t FileSize(int fileFD){
    /**
     * TASK: CALCULATE THE SIZE OF FILE
     * 
     * 1) Move the file pointer to the end of the file, its location
     *    at the end of the file will be the size of the file.
     * 2) Have to rewind back to the beginning after getting the fileSize.
     * 3) fileSize indicate the size of the file in BYTES
    */

    off_t fileSize;

    if ((fileSize = lseek(fileFD, 0, SEEK_END)) < 0){
        perror("Can not get the fileSize:");
        exit(EXIT_FAILURE);
    }
    if (lseek(fileFD, 0, SEEK_SET) < 0){
        perror("Can not rewind back to the beginning of the file:");
        exit(EXIT_FAILURE);
    }

    return fileSize;
}

int File_CreateOpen(const char* filename, int mode){
    
    int fileExe;
    if (mode == 1){
        // M_CREATE
        if ((fileExe = open(filename, O_WRONLY | O_CREAT | O_TRUNC)) < 0){
            perror("Cannot create fileExe:");
            exit(EXIT_FAILURE);
        }
    }
    else {
        //M_OPEN
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