#pragma once

/**
 * Shared library used in both Windows and Linux
 * 
 * 
 * Windows: must use MinGW gcc
*/
    #include <stdlib.h>
    #include <stdio.h>
    #include <unistd.h>
    #include <string.h>
    #include <errno.h>
    #include <stdbool.h>

    #define PORT          6309
    /**
     * If the BANDWIDTH is changed to a value more than 256, the data type of the
     * remainder in the sending and receiving protocol must be changed too. 
     * 
     * The current data type of remainder is uint8_t, so its max is 255 which is just
     * enough to use
    */
    #define BANDWIDTH  256
    #define MESSAGE_QUEUE 200
    

#ifdef _WIN64
    #include <winsock2.h>
    
#endif

#ifdef __unix__
    
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <netinet/in.h>
    #include <sys/select.h>
    #include <sys/sendfile.h>
    #include <fcntl.h>
    
    
#endif

