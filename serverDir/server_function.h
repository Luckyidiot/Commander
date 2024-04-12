#pragma once

#include "../config.h"

/**
 * Fundamental functions of a server
*/
void SetSocket_REUSE(int, int, int, const void*, socklen_t);
struct sockaddr_in Init_IPv4_addr(short, int, char*);
int Create_IPv4Server(short, int, char*, int, int, const void*, socklen_t);
void BindAddr(int, struct sockaddr_in);
void ListenEvent(int, int);
int AcceptConnection(int);
void Closing_procedure(int, int*, fd_set*);




void Write_Message(int, char*);



