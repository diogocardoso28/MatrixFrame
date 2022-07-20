#include <Arduino.h>
#include "FS.h"
#include "filesystem.h"

bool Filesystem::fileSeekCallback(unsigned long position)
{
    return instance->gifFile.seek(position);
}

unsigned long Filesystem::filePositionCallback(void)
{
    return instance->gifFile.position();
}

int Filesystem::fileReadCallback(void)
{
    return instance->gifFile.read();
}

int Filesystem::fileSizeCallback(void)
{
    return instance->gifFile.size();
}

int Filesystem::fileReadBlockCallback(void *buffer, int numberOfBytes)
{
    return instance->gifFile.read((uint8_t *)buffer, numberOfBytes);
}

int Filesystem::openGif(char filePath[])
{
    if (instance->gifFile)
        instance->gifFile.close();

    // Attempt to open the file for reading
    instance->gifFile = SPIFFS.open(filePath, "r");
    if (!instance->gifFile)
    {
        Serial.println("Error opening GIF file");
        return -1;
    }
    return 0;
};