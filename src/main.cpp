#include <Arduino.h>
#include <FastLED.h>
#include <FastLED_GFX.h>
#include <JPEGDecoder.h>

// #include "displayFunctions.h"
#include <FS.h>
#include "gif.h"

#define LED_PIN 2
#define COLOR_ORDER GRB
#define CHIPSET WS2812B
#define BRIGHTNESS 35

#define CANVAS_WIDTH 16
#define CANVAS_HEIGHT 16
#define NUM_LEDS 256

GFXcanvas canvas(16, 16);

CRGB leds[256];

const uint8_t remap[16][16] = {
    {15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0},
    {16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31},
    {47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32},
    {48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63},
    {79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64},
    {80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95},
    {111, 110, 109, 108, 107, 106, 105, 104, 103, 102, 101, 100, 99, 98, 97, 96},
    {112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127},
    {143, 142, 141, 140, 139, 138, 137, 136, 135, 134, 133, 132, 131, 130, 129, 128},
    {144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159},
    {175, 174, 173, 172, 171, 170, 169, 168, 167, 166, 165, 164, 163, 162, 161, 160},
    {176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191},
    {207, 206, 205, 204, 203, 202, 201, 200, 199, 198, 197, 196, 195, 194, 193, 192},
    {208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223},
    {239, 238, 237, 236, 235, 234, 233, 232, 231, 230, 229, 228, 227, 226, 225, 224},
    {240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255}};

void displayJpegMatrix(String path)
{
  Serial.println("Aqui");
  canvas.fillScreen(CRGB::Black);
  FastLED.show();
  delay(1000);

  // canvas.fillScreen(CRGB::Black);
  if (JpegDec.decodeFsFile(path))
  {
    Serial.println("===============");
    Serial.println("JPEG image info");
    Serial.println("===============");
    Serial.print("Width      :");
    Serial.println(JpegDec.width);
    Serial.print("Height     :");
    Serial.println(JpegDec.height);
    Serial.print("Components :");
    Serial.println(JpegDec.comps);
    Serial.print("MCU / row  :");
    Serial.println(JpegDec.MCUSPerRow);
    Serial.print("MCU / col  :");
    Serial.println(JpegDec.MCUSPerCol);
    Serial.print("Scan type  :");
    Serial.println(JpegDec.scanType);
    Serial.print("MCU width  :");
    Serial.println(JpegDec.MCUWidth);
    Serial.print("MCU height :");
    Serial.println(JpegDec.MCUHeight);
    Serial.println("===============");
    Serial.println("");
    Serial.println("Aqui");

    uint32_t mcuPixels = 256;
    uint8_t row = 0;
    uint8_t col = 0;
    uint16_t *pImg;

    uint8_t b;
    uint8_t g;
    uint8_t r;

    int max_x = JpegDec.width, max_y = JpegDec.height;

    while (JpegDec.read())
    {
      pImg = JpegDec.pImage;
      int mcu_x = JpegDec.MCUx;
      for (int x = 0; x < max_y; x++)
      {
        for (int y = 0; y < max_y; y++)
        {
          b = uint8_t((*pImg & 0x001F) << 3) + 7;   // 5 LSB for blue
          g = uint8_t((*pImg & 0x07C0) >> 3) + 7;   // 6 'middle' bits for green
          r = uint8_t((*pImg++ & 0xF800) >> 8) + 7; // 5 MSB for red
          Serial.print("Pixel : ");
          Serial.print(x);
          Serial.print(",");
          Serial.print(y);

          Serial.print(" r: ");
          Serial.print(r);
          Serial.print(" g: ");
          Serial.print(g);
          Serial.print(" b: ");
          Serial.println(b);

          canvas.drawPixel(y, x, CRGB(r, g, b));
          delay(10);

          FastLED.show();
        }
      }
    }
  }
  else
  {
    Serial.println("ERROR");
  }
}

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
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(canvas.getBuffer(), NUM_LEDS).setCorrection(Typical8mmPixel);
  FastLED.setBrightness(BRIGHTNESS);
  // drawBMP(Webp, canvas, CANVAS_WIDTH, CANVAS_HEIGHT);

  displayJpegMatrix("/mario16.jpg");
  FastLED.show();
}

void loop()
{

  // delay(500);
}