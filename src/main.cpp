#include <Arduino.h>
#include <string.h>
#include <FS.h>
#include "displayFunctions.h"
#include "settings.h"
#include "wifi.h"

// Custom Function Files
#include "gammaTable.c"
#include "displayFunctions.h"
#include "filesystem.h"

Filesystem filesystem;
Settings settings;
Wifi wifi;

// Opens gif file
int changed = 0;

void setup()
{
  if (!Serial)
    Serial.begin(9600);

  // iniclizations
  wifi.connect();
  inicializeFastled();
  configGifDecoder();

  Filesystem::openGif("/gif/rainbow.gif");
  // displayJpegMatrix("/mario16.jpg");
}

void loop()
{
  if (millis() > 5000 && changed == 0)
  {
    Filesystem::openGif("/gif/earth.gif");
    changed = 1;
  }
  decodeFrame();
}
