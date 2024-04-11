#pragma once
#include "../config.h"

/**
 * File-MODE Macros
*/
#define M_OPEN   0
#define M_CREATE 1

int File_CreateOpen(const char*, int); //pass the Macros M_OPEN or M_CREATE to the second parameter

void DataProcess(int, int, uint8_t, int);
void Encrypt(char*, int, uint8_t);
void Decrypt(char*, int, uint8_t);