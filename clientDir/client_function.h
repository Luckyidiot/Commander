#pragma once
#include "../config.h"


/**
 * TASK: Establish the connection to the server
*/
void Connect_Server(int, struct sockaddr_in);
struct sockaddr_in Init_IPv4_addr(short, int, char*);
int IPv4_SocketCreate();

/**
 * TASK: Communicate with the server
*/
void Send_attempt(int, int, off_t);
void Read_Message(int, char*, size_t);


