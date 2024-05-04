#include "../config.h"
#include "client_execution.h"
#include "client_function.h"

struct deviceInfo {
    char IPaddr[IP_WIDTH]; // AAA.BBB.CCC.DDD -> 15 chars, but the last one is the null terminator
    char response[DATA_WIDTH];
};

void Get_availDevices(int socketFD, struct timeval timeout, struct deviceInfo* record, size_t dataWidth);
void Signaling(int socketFD, const char* message, struct sockaddr_in scanIP);