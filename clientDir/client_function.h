#pragma once
#include "../config.h"


/**
 * TASK: Establish the connection to the server
*/
void Connect_Server(int socketfd, struct sockaddr_in serverAddress);
struct sockaddr_in Init_IPv4_addr(short sin_family, int port, char* address);
int IPv4_SocketCreate();

/**
 * TASK: Communicate with the server
*/
void Send_attempt(int socketFD, int fileFD, off_t fileSize);
void Read_Message(int buffer_socketfd, char* recvMessage, size_t bandWidth);


