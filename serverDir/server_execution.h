#pragma once
#include "../config.h"


/**
 * SWITCH - ENABLER
*/
#define ENABLE  1
#define DISABLE 0

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
 * Access Permission
*/
#define EXE 1 //Executable
#define REG 0 //Regular


/**
 * Cryptography
*/
void Encrypt(char* data, int length, uint8_t key);
void Decrypt(char* data, int length, uint8_t key);
void Cryptography(const char* filename_src, const char* filename_dst, uint8_t enabler, uint8_t mode, int key);


/**
 * Trivial features
*/
void File_naming(char* filename, size_t length, int socketFD);
void Change_AccessPermission(const char* filename, int mode);

/**
 * TASK: Operations on Files
*/
int File_CreateOpen(const char* filename, int mode); //pass the Macros M_OPEN or M_CREATE to the second parameter