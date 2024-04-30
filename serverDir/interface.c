#include "interface.h"


int Create_IPv4Server(short sin_family, int port, char* address, const int en, int protocol, int option){
    /**
     * TASK: CREATE SOCKET & BIND IT TO AN ADDRESS
     * 
     * const int en :: enable the socket modification, pass 1 for YES, 0 for NO
    */

    int socketFD = IPv4_SocketCreate(protocol);
    struct sockaddr_in serverAddress = Init_IPv4_addr(sin_family, port, address);

    Socket_modify(en, socketFD, option);
    BindAddr(socketFD, serverAddress);

    return socketFD;
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

void Respond_signal(){

    /**
     * TASK: RESPOND TO THE SIGNAL RECEIVE FROM THE CLIENT TO NOTIFY THAT YOU'RE READY TO GET THE FILE
     * 
    */
    char request[DATA_WIDTH];
    fd_set readFDs;
    int serverSocket, maxFD;
    struct sockaddr_in clientIP;
    char repsond[DATA_WIDTH];

/*------------------------------------------------------------------------------------------------*/

    /**
     * TASK: INIT SOME VARIABLES
    */
    serverSocket = Create_IPv4Server(AF_INET, PORT, "INADDR_ANY", 1, UDP, SO_REUSEADDR);
    maxFD = serverSocket + 1;
    FD_ZERO(&readFDs);
    FD_SET(serverSocket, &readFDs);

/*------------------------------------------------------------------------------------------------*/

    /**
     * TASK: HANDLE THE REQUEST AND RESPOND BACK
     * 
     * . Wait until there is a request
     * . Type the message you want to send back to the client
    */
    while (1){
        printf("Waiting for connection...\n");
        
        if (select(maxFD, &readFDs, NULL, NULL, NULL) < 0){
            perror("SELECT ERROR:");
            exit(EXIT_FAILURE);
        }
        printf("select() returns\n");
        

        ReceiveAttempt_message(UDP, serverSocket, request, DATA_WIDTH, &clientIP);

/*------------------------------------------------------------------------------------------------*/

        /**
         * TASK: INIT THE MESSAGE
         * 
         * get the input from the user
        */
        printf("### The client's request is: %s\n", request);
        printf("### Your respond (maximum %d characters) is: ", (DATA_WIDTH - 1));
        scanf("%6s", repsond);
        memset(&repsond[DATA_WIDTH - 1], 0, 1);

/*------------------------------------------------------------------------------------------------*/
        
        SendAttempt_message(UDP, serverSocket, repsond, &clientIP);
        
        printf("Respond sent\n");
    }
    
    close(serverSocket);
}

