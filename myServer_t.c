
#include "serverDir/interface.h"

int main(int argc, char** argv){
    /**
     * The Server will accept the connection and handle the operation required
     * by the client via the binary file received. 
     * 
     * 
    */

    fd_set bufferFDs, readFDs;
    int serverSocket, maxFD;
    const int on = 1;
    

    /**
     * TASK: CREATING THE SERVER
    */
    serverSocket = Create_IPv4Server(AF_INET, PORT, "INADDR_ANY", SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    maxFD = serverSocket;
    printf("serverSocket: %d\n", serverSocket);

    /**
     * TASK: SETTING UP THE LISTENER, EVEN THE select()
    */
    FD_ZERO(&readFDs);
    FD_SET(serverSocket, &readFDs);
    ListenEvent(serverSocket, MESSAGE_QUEUE);
    

    /**
     * Loop to wait until the next return of select()
    */

    while (1){
        bufferFDs = readFDs;
        if (select(maxFD + 1, &bufferFDs, NULL, NULL, NULL) < 0){
            perror("SELECT ERROR:");
            exit(EXIT_FAILURE);
        }
        int bufferMax = maxFD;
        for (int i = 0; i <= bufferMax + 1; i++){

            if (FD_ISSET(i, &bufferFDs)){
                if (i == serverSocket){
                    /**
                     * A new conenction is found
                     * 
                     * Accept it without Handling
                     * 
                     * The first and second parameters of AcceptConnection are for retrieving the address
                     * of the client. Now we temporarily ignore it since we don't need that info.
                    */
                    int bufferSocket = AcceptConnection(i);
                    printf("Connection Accepted....\n");
                    FD_SET(bufferSocket, &readFDs);

                    if (bufferSocket > maxFD){
                        maxFD = bufferSocket;
                    }
                }
                else {
                    /**
                     * The file descriptor is ready to be handle 
                     * 
                    */
                    //Receive_fileExe(i, ENABLE);

                    Write_Message(i, "HelloQuan");
                    Closing_procedure(i, &maxFD, &readFDs);

                }
            }
        }
    }
    
    close(serverSocket);
    return 0;

}