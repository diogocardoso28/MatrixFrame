#include <Arduino.h>
#include <FastLED.h>
#include <FastLED_GFX.h>
#include <GifDecoder.h>
#include <string.h>
#include <FS.h>

#define LED_PIN 2
#define COLOR_ORDER GRB
#define CHIPSET WS2812B
#define BRIGHTNESS 30

#define PANEL_VOLTAGE 5
#define PSU_CURRENT_MILIAMPS 1500

#define CANVAS_WIDTH 16
#define CANVAS_HEIGHT 16

#define NUM_LEDS 256

// Custom Function Files
#include "gammaTable.c"
#include "displayFunctions.h"
#include "filesystem.h"

// Opens gif file

File gifFile;

GifDecoder<CANVAS_WIDTH, CANVAS_HEIGHT, 12> decoder;

void setup()
{
  Serial.begin(9600);

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

  initiateDecoding();
  // displayJpegMatrix("/mario16.jpg");
  openGif("/gif/nyan.gif");
}

void loop()
{
  decodeFrame();
}
