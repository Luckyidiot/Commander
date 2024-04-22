#include "interface.h"


void Send_file(int socketFD, const char* filename, uint8_t cryptoEnabler){

    /**
     * TASK: SENDING FILE TO THE SERVER
     * 
     * 1) Open the file to be sent
     * 2) Encrypt it to another file
     * 3) Use sendfile() to send the content of file descriptor to the
     *    socket
     * 
    */
    int fileOrg;
    int fileEncrypt;
    char filename_encrypt[51];

    //Label the name of the Encrypted file to be sent
    memset(filename_encrypt, 0, sizeof(fileEncrypt));
    sprintf(filename_encrypt, "%s_Encrypt", filename); 

    /**
     * TASK: FILE OPERATION 
     * 
     * 1) Open the file to be sent
     * 2) Create an empty file for encryption
    */
    fileOrg = File_CreateOpen(filename, M_OPEN);
    fileEncrypt = File_CreateOpen(filename_encrypt, M_CREATE);
    
    /**
     * TASK: Change the encrypted file created
    */
    Change_AccessPermission(filename_encrypt, REG);

    /**
     * TASK: ENCRYPTION
     * 
     * read() from fileExe_org
     * Encrypt
     * write() to fileExe_encrypt
    */
    Cryptography(filename, filename_encrypt, cryptoEnabler, M_ENCRYPT, KEY);

    /**
     * TASK: SENDING FILE
     * 
     * Send the file to the server
    */
    SendAttempt_files(socketFD, fileEncrypt, FileSize(fileEncrypt), filename);

    /**
     * TASK: ENDING procedure
     * 
     * 1) Close both file descriptors.
     * 2) Delete ecnrypted file.
    */
    close(fileOrg);
    close(fileEncrypt);
    remove(filename_encrypt);
}

void Scan_devices(){
    /**
     * TASK: SCAN THE AVAILABLE DEVICES WITHIN LAN
     * 
     * OBSTACLE: SOCKET USED FOR THE SENDING FILE CANNOT BE USED FOR SCANNING
    */

    int socketFD;
    char buffer[BANDWIDTH];
    struct sockaddr_in signalAddr;
    fd_set readFDs;
    struct timeval timeout;
    int status;


    socketFD = IPv4_SocketCreate();
    signalAddr = Init_IPv4_addr(AF_INET, PORT, "INADDR_BROADCAST");

    FD_ZERO(&readFDs);
    FD_SET(socketFD, &readFDs);

    timeout.tv_sec = 10;
    timeout.tv_usec = 0;

    Establish_connection(socketFD, signalAddr);

    Signaling(socketFD);

    if ((status = select(1, &readFDs, NULL, NULL, &timeout)) < 0){
        perror("SELECT ERROR:");
        exit(EXIT_FAILURE);
    }
    if (status == 0){
        printf("Scanning timeout\n");
        exit(EXIT_FAILURE);
    }
    printf("Respon received\n");
    
    if (recv(socketFD, buffer, BANDWIDTH, 0) < 0){
        perror("READ MESSAGE: ");
        exit(EXIT_FAILURE);
    }

    printf("%s received", buffer);
}
