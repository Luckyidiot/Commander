#include "interface.h"
#include "client_execution.h"
#include "client_function.h"

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
    int fileExe_org;
    int fileExe_encrypt;
    char* filename_encrypt;

    //Label the name of the Encrypted file to be sent
    sprintf(filename_encrypt, "%s_Encrypt", filename); 

    /**
     * TASK: FILE OPERATION 
     * 
     * 1) Open the file to be sent
     * 2) Create an empty file for encryption
    */
    fileExe_org = File_CreateOpen(filename, M_OPEN);
    fileExe_encrypt = File_CreateOpen(filename, M_CREATE);
    
    /**
     * TASK: ENCRYPTION
     * 
     * read() from fileExe_org
     * Encrypt
     * write() to fileExe_encrypt
    */
    DataProcess(fileExe_org, fileExe_encrypt, KEY, M_ENCRYPT);

    /**
     * TASK: SENDING FILE
     * 
     * Send the file to the server
    */
    Send_attempt(socketfd, fileExe_encrypt, FileSize(fileExe_encrypt));

    /**
     * TASK: ENDING procedure
     * 
     * 1) Close both file descriptors.
     * 2) Delete ecnrypted file.
    */
    close(fileExe_org);
    close(fileExe_encrypt);
    remove(filename_encrypt);
}
