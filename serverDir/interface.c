#include "interface.h"


int Create_IPv4Server(short sin_family, int port, char* address, int level, int optname, const void* optval, socklen_t optlen){
    int socketfd = IPv4_SocketCreate();
    /**
     * struct sockaddr_in serverAddress = Init_IPv4_addr(AF_INET, PORT, "INADDR_ANY");
    */
    
    struct sockaddr_in serverAddress = Init_IPv4_addr(sin_family, port, address);
    BindAddr(socketfd, serverAddress);

    SetSocket_REUSE(socketfd, level, optname, optval, optlen);

    return socketfd;
}

void Receive_fileExe(int socketFD, uint8_t cryptoEnabler){
    /**
     * TASK: RECEIVE THE FILE FROM FROM SOCKET
    */

    int fileRaw;
    int fileDecrypt;
    char filename[43];
    char filename_decrypt[51];

    
    /**
     * TASK: GIVE THE NAME & CREATE THE FILE THAT IS ABOUT TO BE WRITTEN
     * 
     * The name depends by Date&time and Client IP address
    */
    File_naming(filename, sizeof(filename), socketFD);
    memset(filename_decrypt, 0, sizeof(filename_decrypt));
    sprintf(filename_decrypt, "%s_decrypt", filename); 

    /**
     * TASK: CREATE FILE DESCRIPTORS FOR THE RAW AND DECRYPTED FILES
    */
    fileRaw = File_CreateOpen(filename, M_CREATE);
    fileDecrypt = File_CreateOpen(filename_decrypt, M_CREATE);
    
    /**
     * TASK: ADD THE ACCESS PERMISSION TO BOTH NEWLY CREATED FILES
     * 
     * Because both of these two files were just created by those two command above,
     * they do not have any access permission. We need to assign to them.
    */
    Change_AccessPermission(filename, REG);
    Change_AccessPermission(filename_decrypt, REG);
    
    /**
     * TASK: READ THE DATA FROM SOCKET AND WRITE IT TO A FILE
    */
    Receive_attempt(socketFD, fileRaw);
 
    /**
     * TASK: DECRYPT THE RECEIVED FILE
    */
    Cryptography(filename, filename_decrypt, cryptoEnabler, M_DECRYPT, KEY);
    
    /**
     * TASK: ENDING PROCEDURE
    */
    close(fileRaw);
    close(fileDecrypt);
    remove(filename);// Delete the raw file and only retain the decrypted file
}



