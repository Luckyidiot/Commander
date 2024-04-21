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
#define KEY        227
#define M_ENCRYPT  1
#define M_DECRYPT  0

/**
 * File-MODE Macros
*/
#define M_OPEN   0
#define M_CREATE 1

/**
 * Access Permission
*/
#define REG 0
#define EXE 1
/**
 * TASK: Operations on files
*/
off_t FileSize(int fileFD);
int File_CreateOpen(const char* filename, int mode); //pass the Macros M_OPEN or M_CREATE to the second parameter
void Change_AccessPermission(const char* filename, int mode);


/**
 * TASK: CRYPTOGRAPHY
 * 
 * Only call Cryptography(), whether Encrypt() or Decrypt() is determined by the mode
*/
void Encrypt(char* data, int length, uint8_t key);
void Decrypt(char* data, int length, uint8_t key);
void Cryptography(const char* filename_src, const char* filename_dst, uint8_t enabler, uint8_t mode, int key);


