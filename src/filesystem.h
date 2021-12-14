#include <Arduino.h>
#include <FS.h>

extern File gifFile;

bool fileSeekCallback(unsigned long position)
{
    return gifFile.seek(position);
}

unsigned long filePositionCallback(void)
{
    return gifFile.position();
}

int fileReadCallback(void)
{
    return gifFile.read();
}

int fileSizeCallback(void)
{
    return gifFile.size();
}

int fileReadBlockCallback(void *buffer, int numberOfBytes)
{
    return gifFile.read((uint8_t *)buffer, numberOfBytes);
}

int openGif(char filePath[])
{
    if (gifFile)
        gifFile.close();

    // Attempt to open the file for reading
    gifFile = SPIFFS.open(filePath, "r");
    if (!gifFile)
    {
        Serial.println("Error opening GIF file");
        return -1;
    }
}