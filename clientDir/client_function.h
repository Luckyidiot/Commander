#pragma once
#include "../config.h"



/**
 * TASK: ESTABLISH CONNECTION TO THE SERVER
*/
void Establish_connection(int socketfd, struct sockaddr_in serverAddress);
struct sockaddr_in Init_IPv4_addr(short sin_family, int port, char* address);
int IPv4_SocketCreate(int proctocol);
void Socket_modify(const int en, int socketFD, int option);

/**
 * TASK: COMMUNICATION WITH SERVER
*/
void SendAttempt_files(int socketFD, int fileFD, off_t fileSize, const char* filename);
void SendAttempt_any(int socketFD, int fileFD, const char* name);
void SendAttempt_message(int protocol,int socketFD, const char* message, struct sockaddr_in* IP_dst);
void ReceiveAttempt_message(int protocol, int socketFD, char* recvMessage, size_t bandWidth, struct sockaddr_in* IP_src);

/**
 * TASK: SCANNING/RECOGNIZING AVAILABLE DEVICES
*/

void Receive_respon(int socketFD);

