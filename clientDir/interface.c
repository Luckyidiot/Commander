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
    struct timeval timeout;
    pid_t asyncID;
    char credentialMessage[DATA_WIDTH]; // It can only contain 6 characters in reality, because the final element must be null terminator \0
    struct sockaddr_in IPs_list[LST_OFFSET];
    char responses_list[LST_OFFSET][DATA_WIDTH];

/*------------------------------------------------------------------------------------------------*/

    /**
     * TASK: INIT CRUCIAL VARIABLES :: socket and address
     * 
     * 1) socket must be modified to be able to send Broadcast IP
    */
    socketFD = IPv4_SocketCreate(UDP);
    Socket_modify(1, socketFD, SO_BROADCAST);

/*------------------------------------------------------------------------------------------------*/

    /**
     * TASK: INIT/FREE NECESSARY ADDITIONAL VARIABLES
     * 
     * 1) Free the memory of IP and responses containers
     * 2) Get the message that the client want to send to the server
    */

    printf("What is your message for identifying your receiver(s)? (Maximum %d characters): ", (DATA_WIDTH - 1));
    scanf("%6s", credentialMessage);
    memset(&credentialMessage[DATA_WIDTH - 1], 0, 1);
    memset(IPs_list, 0, (LST_OFFSET * sizeof(struct sockaddr_in)));
    memset(&responses_list[0][0], 0, LST_OFFSET * DATA_WIDTH);
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;
    
/*------------------------------------------------------------------------------------------------*/    

    /**
     * TASK: SCANNING AVAIL DEVICES
     * 
     * NOTE: TWO PROCESSES ARE EXECUTED ASYNCHRONOUSLY :: child-process
     * 
     * 1) Send signal to all devices within LAN only.
     * 2) Get all the responses and record their IP.
     * 3) If there is no new upcoming response in 5 secs, the function returns.
    */
    
    asyncID = fork();
    if (asyncID == 0){
        // Sending signal and credentialMessage to server

        signal(SIGTERM, SIG_DFL);
        Signaling(socketFD, credentialMessage, Init_IPv4_addr(AF_INET, PORT, "INADDR_BROADCAST"));
    }
    else {
        // Get the servers' info

        Get_availDevices(socketFD, timeout, IPs_list, &responses_list[0][0], DATA_WIDTH);
        kill(asyncID, SIGTERM);
    }
    

    for (int i = 0; i < LST_OFFSET; i++){
        printf("Response is %s\n", responses_list[i]);
    }

    close(socketFD);
}