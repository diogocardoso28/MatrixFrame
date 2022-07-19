#include <Arduino.h>
#include <string.h>
#include <FS.h>
#include "displayFunctions.h"

// Custom Function Files
#include "gammaTable.c"
#include "displayFunctions.h"
#include "filesystem.h"

// Opens gif file

File gifFile;
int changed = 0;
void setup()
{
  Serial.begin(9600);
  inicializeFastled();
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
    str += dir.fileName();
    str += " / ";
    str += dir.fileSize();
    str += "\r\n";
  }

  Serial.print(str);

  configGifDecoder();
  openGif("/gif/rainbow.gif");
  initiateDecoding();
  // displayJpegMatrix("/mario16.jpg");
}

void loop()
{
  if (millis() > 5000 && changed == 0)
  {
    openGif("/gif/earth.gif");
    // initiateDecoding();
    changed = 1;
  }
  decodeFrame();
}
