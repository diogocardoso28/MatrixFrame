#include <Arduino.h>
#include "FS.h"
#include "filesystem.h"
#include "displayFunctions.h"
Filesystem::Filesystem()
{
    if (!Serial)
        Serial.begin(9600);
    instance = this;
    bool success = SPIFFS.begin();
    if (!success)
    {
        Serial.println("Error mounting the file system");
        return;
    }
    Serial.println("Contudo do Sistema de Ficheiros:");
    String str = "";
    Dir dir = SPIFFS.openDir("/");
    while (dir.next())
    {
        str += dir.fileName() + " / " + dir.fileSize() + "\r\n";
    }
    Serial.println(str);
}

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
    closeFile(instance->gifFile); // Closes current gif File

    // Attempt to open the file for reading
    instance->gifFile = SPIFFS.open(filePath, "r");
    if (!instance->gifFile)
    {
        Serial.println("Error opening GIF file");
        return 1;
    }
    initiateDecoding();
    return 0;
};

File Filesystem::openFile(char filePath[], String operand)
{
    File file;
    file = SPIFFS.open(filePath, operand.c_str());
    return file;
}

void Filesystem::closeFile(File file)
{
    if (instance->gifFile)
        file.close();
}