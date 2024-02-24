#pragma once

#include "../config.h"

/**
 * IPv4_SocketCreate() depends on the first boolean parameter to 
 * determine if it bind the created socket to the address created by the given
 * following parameters:
 * 
 *      In case the bool = false => the remaining parameters do not have any usage,
 *      you can pass to it whatever you want
*/
void SetSocket_REUSE(int, int, int, const void*, socklen_t);
struct sockaddr_in Init_IPv4_addr(short, int, char*);
int Create_IPv4Server(short, int, char*, int, int, const void*, socklen_t);

/**
 * 
 * These functions exclusively belong to the server-side
*/
void BindAddr(int, struct sockaddr_in);
void ListenEvent(int, int);
int AcceptConnection(struct sockaddr_in*, socklen_t, int);
void Closing_procedure(int, int*, fd_set*);


void Receive_fileExe(int, const char*);
void Write_Message(int, char*);



