#pragma once


#include "../config.h"


void Send_fileExe(int, const char*);
void Read_Message(int, char*, size_t);

void Connect_Server(int socketfd, struct sockaddr_in serverAddress);

struct sockaddr_in Init_IPv4_addr(short sin_family, int port, char* address);

int IPv4_SocketCreate();

