#include "../config.h"
#include "client_execution.h"
#include "client_function.h"

void Get_availDevices(int socketFD, struct timeval timeout, struct sockaddr_in* IPs_list, char* deviceNames_list);
void Signaling(int socketFD, const char* message, struct sockaddr_in scanIP);