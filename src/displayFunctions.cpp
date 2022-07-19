#include <Arduino.h>
#include <JPEGDecoder.h>
#include <string.h>
#include <FastLED.h>
#include <FastLED_GFX.h>
#include <GifDecoder.h>

#include "gammaTable.c"
#include "filesystem.h"
#include "displayFunctions.h"

// Global variable declaration
GFXcanvas canvas(CANVAS_WIDTH, CANVAS_HEIGHT);
CRGB leds[NUM_LEDS];

GifDecoder<CANVAS_WIDTH, CANVAS_HEIGHT, 12> decoder;
/*
 * Initializes FastLED library
 *
 *Return void
 */
void inicializeFastled()
{

    FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(canvas.getBuffer(), NUM_LEDS).setCorrection(TypicalSMD5050);
    FastLED.setBrightness(BRIGHTNESS);
    set_max_power_in_volts_and_milliamps(PANEL_VOLTAGE, PSU_CURRENT_MILIAMPS);
}

/*
 * Draws JPEG image on the panel
 * Takes image path as argument
 * Return void
 */
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

/*
 *Draws pixel on screen using canvas
 *Also apllies correction due to impropper pixel position
 * Return void
 */
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

/*
 *Used to clear the screen
 * Return void
 */
void screenClearCallback(void)
{
    canvas.fillScreen(CRGB::Black);
}

/*
 *Used to 'commit' to the panel
 * Return void
 */
void updateScreenCallback(void)
{
    FastLED.show();
}

/*
 * Used to draw pixel on to screen
 * Needs pixel position and red, green and blue values
 * Also aplies gamma correction according to the gamma table
 * Return void
 */
void drawPixelCallback(int16_t x, int16_t y, uint8_t red, uint8_t green, uint8_t blue)
{

    drawPixel(y, x, CRGB(pgm_read_byte(&gamma8[red]), pgm_read_byte(&gamma8[green]), pgm_read_byte(&gamma8[blue])));
}

void configGifDecoder()
{
    // GifDecoder Setup
    decoder.setScreenClearCallback(screenClearCallback);
    decoder.setUpdateScreenCallback(updateScreenCallback);
    decoder.setDrawPixelCallback(drawPixelCallback);

    decoder.setFileSeekCallback(fileSeekCallback);
    decoder.setFilePositionCallback(filePositionCallback);
    decoder.setFileReadCallback(fileReadCallback);

    decoder.setFileReadBlockCallback(fileReadBlockCallback);
    decoder.setFileSizeCallback(fileSizeCallback);

    FastLED.show();
}

void initiateDecoding()
{
    if (decoder.startDecoding() < 0)
    {
        Serial.println("GIF DONE");
    }
}

void decodeFrame()
{
    if (decoder.decodeFrame() < 0)
    {
        // There's an error with this GIF, go to the next one
        Serial.println("Error decoding");
    }
    FastLED.show();

    Serial.println("FRAME");
}