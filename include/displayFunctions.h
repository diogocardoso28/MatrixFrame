#include <Arduino.h>
#include <string.h>
#include <FastLED.h>
#include <FastLED_GFX.h>

#ifndef DISPLAYFUNCTIONS_H_
#define DISPLAYFUNCTIONS_H_

/*Panel Settings*/
/**********************************************/
/*CHANGE according to your pannel and PSU */
#define CANVAS_WIDTH 16
#define CANVAS_HEIGHT 16
#define NUM_LEDS 256

#define BRIGHTNESS 30

#define LED_PIN 2
#define COLOR_ORDER GRB
#define CHIPSET WS2812B

#define PANEL_VOLTAGE 5
#define PSU_CURRENT_MILIAMPS 1500
/**********************************************/
void inicializeFastled();
void displayJpegMatrix(String path);
void drawPixel(int x, int y, CRGB COLOR);
void screenClearCallback(void);
void updateScreenCallback(void);
void drawPixelCallback(int16_t x, int16_t y, uint8_t red, uint8_t green, uint8_t blue);
void configGifDecoder();
void initiateDecoding();
void decodeFrame();
#endif // DISPLAYFUNCTIONS_H_