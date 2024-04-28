#pragma once

/**
 * Shared library used in both Windows and Linux
 * 
 * Windows: must use MinGW gcc
*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>


/**
 * TASK: SCANNING SURROUNDING DEVICES
 * 
*/
#define LST_OFFSET 20 // Max numbers of devices that can be recorded at once :: May be scanned multiple times
#define MAX_CHAR 4    // Max numbers of characters for device's name :: User will specify

/**
 * TASK: ENCAPSULATE THE NAMES WITH MORE SPECIFIC AND COMMON NAMES
*/
#define TCP SOCK_STREAM
#define UDP SOCK_DGRAM

/**
 * TASK: used for establishing conenction
*/
#define PORT          6309
#define BANDWIDTH     256
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
    #include <sys/stat.h>
    #include <time.h>
#endif

