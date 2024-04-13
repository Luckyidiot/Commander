#pragma once
#include "../config.h"
#include <time.h>

/**
 * Encrypt/Decrypt
*/
#define KEY       227
#define M_ENCRYPT 1
#define M_DECRYPT 0

/**
 * File-MODE Macros
*/
#define M_OPEN    0
#define M_CREATE  1


/**
 * Cryptography
*/
void Encrypt(char*, int, uint8_t);
void Decrypt(char*, int, uint8_t);
void DataProcess(int, int, uint8_t, int);


/**
 * Trivial features
*/
void File_naming(char*, size_t, int);
void ChangeMode(const char*);

/**
 * TASK: Operations on Files
*/
int File_CreateOpen(const char*, int); //pass the Macros M_OPEN or M_CREATE to the second parameter