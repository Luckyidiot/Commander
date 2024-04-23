#include "interface.h"
#include <signal.h>

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
    int socketFD, maxFD, status;
    struct sockaddr_in scanIP, detectedIPs;
    fd_set readFDs;
    struct timeval timeout;
    char buffer[3];
    const char message[3] = "27";


    /**
     * TASK: INIT CRUCIAL VARIABLES :: socket and address
     * 
     * 1) socket must be modified to be able to send Broadcast IP
    */
    socketFD = IPv4_SocketCreate(UDP);
    Socket_modify(1, socketFD, SO_BROADCAST);
    scanIP = Init_IPv4_addr(AF_INET, PORT, "INADDR_BROADCAST");

    /**
     * TASK: INIT NECESSARY ADDITIONAL VARIABLES
     * 
     * 1) readFDs, select(), timeout, and maxFD are used to limit the amount of time scanning is going
     *    to wait for responding. 
    */
    FD_ZERO(&readFDs);
    FD_SET(socketFD, &readFDs);
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;
    maxFD = socketFD + 1;
    
    SendAttempt_message(UDP, socketFD, message, &scanIP);

    printf("Scanning...\n");
    while(1){
        if ((status = select(maxFD, &readFDs, NULL, NULL, &timeout)) <= 0){
            if (status == 0){
                fprintf(stdout, "SCANNING TIMEOUT\n");
                return;
            }
            else {
                perror("SELECT ERROR:");
                exit(EXIT_FAILURE);
            }
        }
        printf("Select() return\n");
        ReceiveAttempt_message(UDP, socketFD, buffer, 3, &detectedIPs);
        printf("%s\n", buffer);
        memset(buffer, 0, 3);
    }
    printf("Finish scanning\n");
    
}
