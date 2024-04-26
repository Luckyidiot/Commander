#include "../config.h"
#include "server_execution.h"
#include "server_function.h"

int Create_IPv4Server(short sin_family, int port, char* address, const int en, int protocol, int option);
void Receive_fileExe(int socketFD, uint8_t cryptoEnabler);
void Respond_signal();