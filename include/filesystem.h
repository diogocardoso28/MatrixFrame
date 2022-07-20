#include <Arduino.h>

#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

class Filesystem
{
    static inline Filesystem *instance;

public:
    Filesystem()
    {
        instance = this;
    }

    static bool fileSeekCallback(unsigned long position);
    static unsigned long filePositionCallback(void);
    static int fileReadCallback(void);
    static int fileSizeCallback(void);
    static int fileReadBlockCallback(void *buffer, int numberOfBytes);
    static int openGif(char filePath[]);

private:
    File gifFile;
};
#endif // FILESYSTEM_H_