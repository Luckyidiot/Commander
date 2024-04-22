#pragma once

#include "../config.h"

/**
 * Fundamental functions of a server
*/
void SetSocket_REUSE(int sockfd, int level, int optname, const void* optval, socklen_t optlen);
struct sockaddr_in Init_IPv4_addr(short sin_family, int port, char* address);
int IPv4_SocketCreate();
void BindAddr(int socketfd, struct sockaddr_in addr);
void ListenEvent(int socketfd, int queueLength);
int AcceptConnection(int socketfd);
void Closing_procedure(int fd, int* maxFD, fd_set* readFDs);


void Receive_attempt(int socketFD, int fileDST);

void Write_Message(int socketfd, char* message);



