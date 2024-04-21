#pragma once
#include "../config.h"
#include "client_execution.h"
#include "client_function.h"

void Send_file(int socketFD, const char* filename, uint8_t cryptoEnabler);

void Scan_devices();
