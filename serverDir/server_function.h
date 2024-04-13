#pragma once

#include "../config.h"

/**
 * Fundamental functions of a server
*/
void SetSocket_REUSE(int, int, int, const void*, socklen_t);
struct sockaddr_in Init_IPv4_addr(short, int, char*);
int IPv4_SocketCreate();
void BindAddr(int, struct sockaddr_in);
void ListenEvent(int, int);
int AcceptConnection(int);
void Closing_procedure(int, int*, fd_set*);


void Receive_attempt(int, int);

void Write_Message(int, char*);



