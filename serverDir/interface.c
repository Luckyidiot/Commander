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

void Receive_fileExe(int socketfd){
    /**
     * TASK: RECEIVE THE FILE FROM FROM SOCKET
    */

    int fileExe;
    int fileDecrypt;
    char filename[43];
    char filename_decrypt[51];

    
    /**
     * TASK: GIVE THE NAME & CREATE THE FILE THAT IS ABOUT TO BE WRITTEN
     * 
     * The name depends by Date&time and Client IP address
    */
    File_naming(filename, sizeof(filename), socketfd);
    memset(filename_decrypt, 0, sizeof(filename_decrypt));
    sprintf(filename_decrypt, "%s_decrypt", filename); 
    printf("fileName is %s\n", filename_decrypt);

    /**
     * TASK: CREATE FILE DESCRIPTORS FOR THE RAW AND DECRYPTED FILES
    */
    fileExe = File_CreateOpen(filename, M_CREATE);
    fileDecrypt = File_CreateOpen(filename_decrypt, M_CREATE);


    /**
     * TASK: READ THE DATA FROM SOCKET AND WRITE IT TO A FILE
    */
    Receive_attempt(socketfd, fileExe);

    /**
     * TASK: DECRYPT THE RECEIVED FILE
    */
    DataProcess(fileExe, fileDecrypt, KEY, M_DECRYPT);

    /**
     * TASK: ENDING PROCEDURE
    */
    ChangeMode(filename_decrypt);
    close(fileExe);
    remove(filename);// Delete the raw file and only retain the decrypted file
}

