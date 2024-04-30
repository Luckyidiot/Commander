#include "../config.h"
#include "client_execution.h"
#include "client_function.h"

void Get_availDevices(int socketFD, struct timeval timeout, struct sockaddr_in* IPs_list, char* responses_list, size_t dataWidth);
void Signaling(int socketFD, const char* message, struct sockaddr_in scanIP);