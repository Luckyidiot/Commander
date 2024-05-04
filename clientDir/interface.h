#pragma once
#include <signal.h>

#include "client_execution.h"
#include "client_function.h"
#include "middle_interface.h"


void Send_file(int socketFD, const char* filename, uint8_t cryptoEnabler);


/**
 * TASK: SCAN AND RECORD SURROUNDING DEVICES
 * 
*/
void Scan_devices();
