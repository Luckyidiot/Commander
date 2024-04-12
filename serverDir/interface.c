#include "interface.h"
#include "server_execution.h"
#include "server_function.h"

void Receive_fileExe(int socketfd){
    /**
     * TASK: RECEIVE THE FILE FROM FROM SOCKET
    */

    int fileExe;
    int file_decrypt;
    char filename[43];
    char filename_decrypt[51];

    
    /**
     * TASK: GIVE THE NAME & CREATE THE FILE THAT IS ABOUT TO BE WRITTEN
     * 
     * The name depends by Date&time and Client IP address
    */
    File_naming(filename, sizeof(filename), socketfd);
    memset(filename_decrypt, 0, sizeof(filename_decrypt));
    sprintf(filename_decrypt, "%s_Decrypt", filename); 
    printf("fileName is %s\n", filename_decrypt);

    /**
     * TASK: CREATE FILE DESCRIPTORS FOR THE RAW AND DECRYPTED FILES
    */
    fileExe = File_CreateOpen(filename, M_CREATE);
    file_decrypt = File_CreateOpen(filename_decrypt, M_CREATE);


    /**
     * TASK: READ THE DATA FROM SOCKET AND WRITE IT TO A FILE
    */
    Receive_attempt(socketfd, fileExe);

    /**
     * TASK: DECRYPT THE RECEIVED FILE
    */
    DataProcess(fileExe, file_decrypt, KEY, M_DECRYPT);

    /**
     * TASK: ENDING PROCEDURE
    */
    ChangeMode(filename_decrypt);
    close(fileExe);
    remove(filename);// Delete the raw file and only retain the decrypted file
}