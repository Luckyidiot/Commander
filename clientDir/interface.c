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
    int socketFD, maxFD, terminate;
    struct sockaddr_in scanIP, detectedIPs;
    fd_set readFDs;
    struct timeval timeout;
    pid_t asyncID;
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
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;
    maxFD = socketFD + 1;
    
    /**
     * TASK: SCANNING AVAIL DEVICES
     * 
     * 1) Send signal to all devices within LAN only.
     * 2) Get all the responses and record their IP.
     * 3) If there is no new upcoming response in 5 secs, the function returns.
    */
    
    asyncID = fork();
    if (asyncID == 0){
        signal(SIGTERM, SIG_DFL);
        while (1){
            SendAttempt_message(UDP, socketFD, message, &scanIP);
            printf("Scanning...\n");
            sleep(2);
        }
        
    }
    else {
        while (1){
            if ((terminate = select(maxFD, &readFDs, NULL, NULL, &timeout)) <= 0){
                if (terminate == 0){
                    fprintf(stdout, "SCANNING TIMEOUT\n");
                    break;
                }
                else {
                    perror("SELECT ERROR:");
                    exit(EXIT_FAILURE);
                }
            }
            printf("Response coming\n");
            ReceiveAttempt_message(UDP, socketFD, buffer, 3, &detectedIPs);
            printf("%s\n", buffer);
            memset(buffer, 0, 3);
        }
        kill(asyncID, SIGTERM);
        printf("Finish scanning\n");
    }
    
}

void Get_availDevices(int socketFD, struct timeval timeout, struct sockaddr_in* IPs_list, char** deviceNames_list ){
    int terminate;
    int maxFD;
    fd_set readFDs;
    char deviceName[BANDWIDTH];
    struct sockaddr_in detectedIP;


    FD_ZERO(&readFDs);
    FD_SET(socketFD, &readFDs);
    maxFD = socketFD + 1;
    memset(deviceName, 0, BANDWIDTH);


    while (1){
        if ((terminate = select(maxFD, &readFDs, NULL, NULL, &timeout)) <= 0){
            if (terminate == 0){
                fprintf(stdout, "SCANNING TIMEOUT\n");
                break;
            }
            else {
                perror("SELECT ERROR:");
                exit(EXIT_FAILURE);
            }
        }
        printf("Response coming\n");
        ReceiveAttempt_message(UDP, socketFD, deviceName, BANDWIDTH, &detectedIP);
        
        //Add the detected IP to list
        
        memset(deviceName, 0, BANDWIDTH);
    }
        
    printf("Finish scanning\n");
}

void AsyncExecution(){

}

