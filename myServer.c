
#include "serverDir/interface.h"

void Respond_signal(){
    char buffer[3];
    fd_set readFDs;
    int serverSocket, maxFD;
    struct sockaddr_in clientIP;
    char* repsond = "36";


    serverSocket = Create_IPv4Server(AF_INET, PORT, "INADDR_ANY", 1, UDP, SO_REUSEADDR);
    maxFD = serverSocket + 1;
    printf("serverSocket: %d\n", serverSocket);

    FD_ZERO(&readFDs);
    FD_SET(serverSocket, &readFDs);

    while (1){
        printf("Waiting for connection...\n");
        
        if (select(maxFD, &readFDs, NULL, NULL, NULL) < 0){
            perror("SELECT ERROR:");
            exit(EXIT_FAILURE);
        }
        printf("select() returns\n");
        
        ReceiveAttempt_message(UDP, serverSocket, buffer, 3, &clientIP);
        printf("%s\n", buffer);

        
        SendAttempt_message(UDP, serverSocket, repsond, &clientIP);
        
        printf("Respond sent\n");
    }
    
    close(serverSocket);
}

int main(int argc, char** argv){
   
    Respond_signal();
    return 0;
    
}