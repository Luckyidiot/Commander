#pragma once
#include "../config.h"



/**
 * TASK: ESTABLISH CONNECTION TO THE SERVER
*/
void Establish_connection(int socketfd, struct sockaddr_in serverAddress);
struct sockaddr_in Init_IPv4_addr(short sin_family, int port, char* address);
int IPv4_SocketCreate();

/**
 * TASK: COMMUNICATION WITH SERVER
*/
void SendFile_attempt(int socketFD, int fileFD, off_t fileSize);
void Read_Message(int buffer_socketfd, char* recvMessage, size_t bandWidth);

/**
 * TASK: SCANNING/RECOGNIZING AVAILABLE DEVICES
*/
void Signaling(int socketFD);

