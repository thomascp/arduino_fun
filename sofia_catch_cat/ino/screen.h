#ifndef SCREEN_H
#define SCREEN_H

#include "Arduino.h"
#include "TFT_22_ILI9225.h"
#include "common.h"

#define TFT_RST 8
#define TFT_RS  9
#define TFT_CS  10  // SS
#define TFT_SDI 11  // MOSI
#define TFT_CLK 13  // SCK
#define TFT_LED 3   // 0 if wired to +5V directly

class screen {
    public:
        screen();
        void clean();
        void cleanArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
        void drawColorPixelmap(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t *bitmap);
        void showLines(struct lines *lines, uint16_t num);
        void printText(uint16_t x, uint16_t y, STRING buf, uint8_t* font, uint16_t bg, uint16_t ft);
    private:
        uint16_t backgroundColor;
        TFT_22_ILI9225 tft = TFT_22_ILI9225(TFT_RST, TFT_RS, TFT_CS, TFT_LED, 0);
};

#endif