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

void Get_availDevices(int socketFD, struct timeval timeout, struct deviceInfo* record, size_t dataWidth){
    /**
     * TASK: READ THE INCOMING RESPONSES, RECORD THEIR NAMES AND IP.
     * NOTE: THE LAST ELEMENTS OF responses_list 2D ARRAY ARE NULL \0
     * 
     * 1) select() will expire after an amount of time specified by timeout, regardless of the loop
     * 2) We cannot compare the IP in the type struct sockaddr_in so we have to convert it into the human-readable format, that
     *    is why we have IPaddr[IP_WIDTH]
    */

    int terminate, maxFD;
    fd_set readFDs;
    char IPaddr[IP_WIDTH];
    struct sockaddr_in IP_buffer;
    char response[dataWidth];

/*------------------------------------------------------------------------------------------------*/

    /**
     * TASK: INIT VARS
     * 
    */
    FD_ZERO(&readFDs);
    FD_SET(socketFD, &readFDs);
    maxFD = socketFD + 1;

/*------------------------------------------------------------------------------------------------*/

    for (int i = 0; i < LST_OFFSET; i++){

        /**
         * TASK: WAITING FOR THE REPONSES FROM DEVICES
         * 
         * The timeout passed into select() also determines when the entire function end.
        */
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

                /*-------------------------------------------------------------*/

        ReceiveAttempt_message(UDP, socketFD, response, (dataWidth - 1), &IP_buffer);
        inet_ntop(AF_INET, &IP_buffer.sin_addr, IPaddr, (IP_WIDTH - 1));

                /*-------------------------------------------------------------*/

        /**
         * TASK: SORT THE RESPONSES TO ENSURE THE SAME DEVICE IS NOT RECORDED TWICE.
         * 
        */
        for (int i = 0; i < LST_OFFSET; i++){
            if (record[i].IPaddr[0] == 0){
                memcpy(record[i].IPaddr, IPaddr, IP_WIDTH);
                memcpy(record[i].response, response, dataWidth); 
                printf("New device, added\n");
                break;
            }
            else if (strcmp(record[i].IPaddr, IPaddr) == 0){
                memset(&IPaddr, 0, IP_WIDTH);
                memset(&response, 0, dataWidth);
                printf("Same respond, discard\n");
                break;
            }
        }

    }
    printf("Finish scanning\n");
}