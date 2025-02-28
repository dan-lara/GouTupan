// epd_text.h
#ifndef EPD_TEXT_H
#define EPD_TEXT_H

#include <Arduino.h>
#include "epd2in9b_V3.h"
#include "font8x8.h"

#define CHAR_WIDTH 8
#define CHAR_HEIGHT 8
#define LINE_SPACING 2

class EpdText {
public:
    EpdText(Epd &epd);
    void init();
    void displayText(const char* text);
    void clear();
    void updateDisplay(float temp, float humidity, float pressure = 0); // 接口保留参数

private:
    Epd &_epd;
    UBYTE _buffer[EPD_WIDTH * EPD_HEIGHT / 8];
    
    void drawChar(int x, int y, char c);
    void newlineHandler(int &x, int &y);
};

#endif