#include "clientExe.h"
#include <unistd.h>
#include <stdio.h>


int main(void){
    /**
     * This function is defined in the object file from client
     * 
    */
    clientExe();
    
    /**
     * We need a pipe to send back the output of that clientExe() to 
     * the server.
    */
    
    return 0;

}

/**
 * This file is a TEMPLATE
 * Any modification, even the slightest, can lead to FATAL error
 * 
 * The client file must be an object file.
 * The client source file is a source file .c with the #include "clientExe.h"
 * with a single function named clientExe()
*/