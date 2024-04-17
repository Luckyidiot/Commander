#include "../config.h"
#include "server_execution.h"
#include "server_function.h"

int Create_IPv4Server(short sin_family, int port, char* address, int level, int optname, const void* optval, socklen_t optlen);
void Receive_fileExe(int socketFD, uint8_t cryptoEnabler);