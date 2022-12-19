#include <Arduino.h>
#include <string.h>
#include "FS.h"
#include <WiFi.h>

// Custom Function Files
#include "gammaTable.c"
#include "settings.h"
#include "displayFunctions.h"
#include "filesystem.h"
#include "webserver.h"

Filesystem filesystem;
Settings settings;
Webserver webserver;

// Opens gif file
int changed = 0;

void setup()
{
  if (!Serial)
    Serial.begin(9600);

  // iniclizations
  webserver.startWebServer();
  inicializeFastled();
  configGifDecoder();

  Filesystem::openGif("/gif/rainbow.gif");
  // displayJpegMatrix("/mario16.jpg");
}

void loop()
{
  if (millis() > 50000 && changed == 0)
  {
    Filesystem::openGif("/gif/earth.gif");
    changed = 1;
  }
  decodeFrame();
}
