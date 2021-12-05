#include "Arduino.h"
#include <FastLED.h>
#include <FastLED_GFX.h>
#include <FS.h>
#include <JPEGDecoder.h>

void drawBMP(uint16_t bitMap[16][16], GFXcanvas canvas, int matrixWidth, int matrixHeight)
{
    uint16_t bitMapCurrentColor;
    uint16_t R, G, B;

    // Clears Screen
    canvas.fillScreen(CRGB::Black);
    FastLED.show();

    for (int y = 0; y < matrixHeight; y++) // Cycles trough the y Azis (Height)
    {
        for (int x = 0; x < matrixHeight; x++) // Cycles trough the x Azis (Width)
        {
            bitMapCurrentColor = bitMap[x][y];
            B = bitMapCurrentColor / 65536;
            G = (bitMapCurrentColor - B * 65536) / 256;
            R = bitMapCurrentColor - B * 65536 - G * 256;
            canvas.drawPixel(x, y, CRGB(R, G, B));
        }
    }
}

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
    // canvas.fillScreen(CRGB::Black);
    if (JpegDec.decodeFsFile(path))
    {
        uint32_t mcuPixels = 256;
        uint8_t row = 0;
        uint8_t col = 0;

        while (JpegDec.read())
        {
            uint16_t *pImg = JpegDec.pImage;
            for (uint8_t i = 0; i < mcuPixels; i++)
            {

                // Extract the red, green, blue values from each pixel
                uint8_t b = uint8_t((*pImg & 0x001F) << 3);   // 5 LSB for blue
                uint8_t g = uint8_t((*pImg & 0x07C0) >> 3);   // 6 'middle' bits for green
                uint8_t r = uint8_t((*pImg++ & 0xF800) >> 8); // 5 MSB for red

                // Calculate the matrix index (column and row)
                col = JpegDec.MCUx * 8 + i % 8;
                row = JpegDec.MCUy * 8 + i / 8;
                // Set the matrix pixel to the RGB value
                // pixels.setPixelColor(remap[row][col], pixels.Color(r, g, b));
                // canvas.drawPixel(row, col, CRGB(r, g, b));
                canvas[remap[row][col]].red = r;
                canvas[remap[row][col]].green = g;
                canvas[remap[row][col]].blue = b;
            }
        }
        // pixels.show();
    }
    else
    {
        Serial.println("ERROR");
    }
}