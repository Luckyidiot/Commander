#pragma once

#include "../config.h"

/**
 * Fundamental functions of a server
*/
void Socket_modify(const int en, int socketFD, int option);
struct sockaddr_in Init_IPv4_addr(short sin_family, int port, char* address);
int IPv4_SocketCreate(int proctocol);
void BindAddr(int socketfd, struct sockaddr_in addr);
void ListenEvent(int socketfd, int queueLength);
int AcceptConnection(int socketfd);
void Closing_procedure(int fd, int* maxFD, fd_set* readFDs);


void Receive_attempt(int socketFD, int fileDST);

void Write_Message(int socketfd, char* message);

void ReceiveAttempt_message(int protocol, int socketFD, char* recvMessage, size_t bandWidth, struct sockaddr_in* IP_src);
void SendAttempt_message(int protocol, int socketFD, const char* message, struct sockaddr_in* IP_dst);



