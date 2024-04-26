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
    if (mode){
        // M_CREATE
        if ((fileExe = open(filename, O_RDWR | O_CREAT | O_TRUNC)) < 0){
            perror("Cannot create fileExe:");
            exit(EXIT_FAILURE);
        }
    }
    else {
        //M_OPEN
        if ((fileExe = open(filename, O_RDWR)) < 0){
            perror("Cannot open fileExe:");
            exit(EXIT_FAILURE);
        }
    }

    return fileExe;
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
            perror("ERROR: Client's cryptography failed");
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

void Notifier_alarm(int signum){
    terminate = 0;
}

