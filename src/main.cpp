#include <Arduino.h>
#include <FastLED.h>
#include <FastLED_GFX.h>
#include <JPEGDecoder.h>
#include <GifDecoder.h>
#include <string.h>
#include <FS.h>

#define LED_PIN 2
#define COLOR_ORDER GRB
#define CHIPSET WS2812B
#define BRIGHTNESS 30

#define CANVAS_WIDTH 16
#define CANVAS_HEIGHT 16

#define NUM_LEDS 256

GFXcanvas canvas(16, 16);

CRGB leds[256];
File file;

const uint8_t PROGMEM gamma8[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2,
    2, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5,
    5, 6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10,
    10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
    17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
    25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
    37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
    51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
    69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
    90, 92, 93, 95, 96, 98, 99, 101, 102, 104, 105, 107, 109, 110, 112, 114,
    115, 117, 119, 120, 122, 124, 126, 127, 129, 131, 133, 135, 137, 138, 140, 142,
    144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164, 167, 169, 171, 173, 175,
    177, 180, 182, 184, 186, 189, 191, 193, 196, 198, 200, 203, 205, 208, 210, 213,
    215, 218, 220, 223, 225, 228, 231, 233, 236, 239, 241, 244, 247, 249, 252, 255};

GifDecoder<CANVAS_WIDTH, CANVAS_HEIGHT, 12> decoder;
void drawPixel(int x, int y, CRGB COLOR);

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

          canvas.drawPixel(y, x, CRGB(pgm_read_byte(&gamma8[r]), pgm_read_byte(&gamma8[g]), pgm_read_byte(&gamma8[b])));
        }
      }
      FastLED.show();
    }
  }
  else
  {
    Serial.println("ERROR");
  }
}
void screenClearCallback(void)
{
  canvas.fillScreen(CRGB::Black);
}

void updateScreenCallback(void)
{
  FastLED.show();
}

void drawPixelCallback(int16_t x, int16_t y, uint8_t red, uint8_t green, uint8_t blue)
{

  drawPixel(y, x, CRGB(pgm_read_byte(&gamma8[red]), pgm_read_byte(&gamma8[green]), pgm_read_byte(&gamma8[blue])));
}

bool fileSeekCallback(unsigned long position)
{
  return file.seek(position);
}

unsigned long filePositionCallback(void)
{
  return file.position();
}

int fileReadCallback(void)
{
  return file.read();
}

int fileSizeCallback(void)
{
  return file.size();
}

int fileReadBlockCallback(void *buffer, int numberOfBytes)
{
  return file.read((uint8_t *)buffer, numberOfBytes);
}

void drawPixel(int x, int y, CRGB COLOR)
{
  if (y == 0 || y % 2 == 0)
  {
    canvas.drawPixel(x, y, COLOR);
  }
  else
  {
    canvas.drawPixel((CANVAS_WIDTH - x) - 1, y, COLOR);
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
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(canvas.getBuffer(), NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  set_max_power_in_volts_and_milliamps(5, 1500);

  // GifDecoder Setup
  decoder.setScreenClearCallback(screenClearCallback);
  decoder.setUpdateScreenCallback(updateScreenCallback);
  decoder.setDrawPixelCallback(drawPixelCallback);

  decoder.setFileSeekCallback(fileSeekCallback);
  decoder.setFilePositionCallback(filePositionCallback);
  decoder.setFileReadCallback(fileReadCallback);

  decoder.setFileReadBlockCallback(fileReadBlockCallback);
  decoder.setFileSizeCallback(fileSizeCallback);

  // displayJpegMatrix("/mario16.jpg");

  if (file)
    file.close();

  // Attempt to open the file for reading
  file = SPIFFS.open("/gif/nyan.gif", "r");
  if (!file)
  {
    Serial.println("Error opening GIF file");
    // return -1;
  }

  FastLED.show();
}

void loop()
{
  static unsigned long displayStartTime_millis;
  static int nextGIF = 1; // we haven't loaded a GIF yet on first pass through, make sure we do that

  unsigned long now = millis();

  static int index = 0;

  if (index == 0)
  {

    if (decoder.startDecoding() < 0)
    {
      Serial.println("GIF DONE");
    }
    index++;
  }
  // delay();

  if (decoder.decodeFrame() < 0)
  {
    // There's an error with this GIF, go to the next one
    Serial.println("Error decoding");
  }
  // index++;
  // file.close();
}
