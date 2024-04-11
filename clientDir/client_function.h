#pragma once
#include "../config.h"
#include <stdbool.h>

/**
 * These are basic functions to connect to server
*/
void Connect_Server(int, struct sockaddr_in);
struct sockaddr_in Init_IPv4_addr(short, int, char*);
int IPv4_SocketCreate();


void Send_attempt(int, int, off_t);
off_t FileSize(int);

void Send_fileExe(int, const char*);
void Read_Message(int, char*, size_t);


