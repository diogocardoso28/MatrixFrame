#include <Arduino.h>
#include "FS.h"

#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

class Filesystem
{
    static inline Filesystem *instance;

public:
    Filesystem();
    static bool fileSeekCallback(unsigned long position);
    static unsigned long filePositionCallback(void);
    static int fileReadCallback(void);
    static int fileSizeCallback(void);
    static int fileReadBlockCallback(void *buffer, int numberOfBytes);
    static int openGif(char filePath[]);
    static File openFile(char filePath[], String operand = "r");
    static void closeFile(File file);

private:
    File gifFile;
};
#endif // FILESYSTEM_H_