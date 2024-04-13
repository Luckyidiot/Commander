#include "interface.h"


void Send_fileExe(int socketfd, const char* filename){

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
    fileEncrypt = File_CreateOpen(filename, M_CREATE);
    
    /**
     * TASK: ENCRYPTION
     * 
     * read() from fileExe_org
     * Encrypt
     * write() to fileExe_encrypt
    */
    DataProcess(fileOrg, fileEncrypt, KEY, M_ENCRYPT);

    /**
     * TASK: SENDING FILE
     * 
     * Send the file to the server
    */
    Send_attempt(socketfd, fileOrg, FileSize(fileOrg));

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
