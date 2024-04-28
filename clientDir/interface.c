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

void Get_availDevices(int socketFD, struct timeval timeout, struct sockaddr_in* IPs_list, char* deviceNames_list){
    /**
     * TASK: READ THE INCOMING RESPONSES, RECORD THEIR NAMES AND IP.
     * 
     * 1) select() will expire after an amount of time specified by timeout, regardless of the loop
     * 2) char* deviceNames_list :: the pointer to the first element in the two dimentional array,
     *    deviceNames_list[LST_OFFSET][MAR_CHAR];
    */

    int terminate, maxFD;
    fd_set readFDs;


    FD_ZERO(&readFDs);
    FD_SET(socketFD, &readFDs);
    maxFD = socketFD + 1;


    for (int i = 0; i < LST_OFFSET; i++){
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
        ReceiveAttempt_message(UDP, socketFD, deviceNames_list, MAX_CHAR, IPs_list);
        IPs_list += 1;
        deviceNames_list += MAX_CHAR;
    }
    printf("Finish scanning\n");
}

void Signaling(int socketFD, const char* message, struct sockaddr_in scanIP){
    /**
     * TASK: Sending signal to the aim machine to connect to using UDP
     * PROTOCOL: UDP
     * 
     * NOTE: If you want to perform scanning mechanics like a rada, pass Broadcast address
     * with Init_IPv4_addr() into scanIP
     * 
    */
    while (1){
        SendAttempt_message(UDP, socketFD, message, &scanIP);
        printf("Scanning...\n");
        sleep(1);
    }
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
    struct sockaddr_in IPs_list[LST_OFFSET];
    char deviceNames_list[LST_OFFSET][MAX_CHAR];

    /**
     * TASK: INIT CRUCIAL VARIABLES :: socket and address
     * 
     * 1) socket must be modified to be able to send Broadcast IP
    */
    socketFD = IPv4_SocketCreate(UDP);
    Socket_modify(1, socketFD, SO_BROADCAST);

    /**
     * TASK: INIT/FREE NECESSARY ADDITIONAL VARIABLES
     * 
     * 1) readFDs, select(), timeout, and maxFD are used to limit the amount of time scanning is going
     *    to wait for responding. 
    */
    memset(IPs_list, 0, LST_OFFSET);
    memset(&deviceNames_list[0][0], 0, LST_OFFSET * MAX_CHAR);
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;
    
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
        Signaling(socketFD, "27", Init_IPv4_addr(AF_INET, PORT, "INADDR_BROADCAST"));
        
    }
    else {
        Get_availDevices(socketFD, timeout, IPs_list, &deviceNames_list[0][0]);
        kill(asyncID, SIGTERM);
    }
    
}