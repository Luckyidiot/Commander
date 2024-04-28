#include "middle_interface.h"

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