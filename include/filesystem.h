#include <Arduino.h>

#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

// Code for header body
bool fileSeekCallback(unsigned long position);
unsigned long filePositionCallback(void);
int fileReadCallback(void);
int fileSizeCallback(void);
int fileReadBlockCallback(void *buffer, int numberOfBytes);
int openGif(char filePath[]);
#endif // FILESYSTEM_H_