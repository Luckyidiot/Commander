#pragma once
#include "../config.h"
#include "client_execution.h"
#include "client_function.h"
#include <signal.h>


void Send_file(int socketFD, const char* filename, uint8_t cryptoEnabler);


/**
 * TASK: SCAN AND RECORD SURROUNDING DEVICES
 * 
*/
void Get_availDevices(int socketFD, struct timeval timeout, struct sockaddr_in* IPs_list, char* deviceNames_list);
void Signaling(int socketFD, const char* message, struct sockaddr_in scanIP);
void Scan_devices();
