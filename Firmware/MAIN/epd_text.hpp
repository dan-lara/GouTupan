#ifndef EPD_TEXT_H
#define EPD_TEXT_H

#include <Arduino.h>
#include "epd2in9b_V3.hpp"
#include "font8x8.hpp"
#include <stdlib.h>

#define CHAR_WIDTH 8
#define CHAR_HEIGHT 8
#define LINE_SPACING 2

class EpdText {
public:
    EpdText(Epd &epd);
    void init();
    void displayText(const char* text);
    void clear();
    void updateDisplay(
        float mux_code = 0, float outside_temperature = 0, float outside_CO2 = 0, float outside_humidity = 0, float battery_level = 0,
        float soil_nutrients_N_Nitrogen = 0, float soil_nutrients_P_Phosphorus = 0, float soil_nutrients_K_Potassium = 0,
        float surface_temperature = 0, float surface_humidity = 0,
        float deep_temperature = 0, float deep_humidity = 0,
        float light_intensity = 0, float light_infrared = 0, float light_ultraviolet = 0,
        float R_RGB = 0, float G_RGB = 0, float B_RGB = 0,
        float pressure = 0, float quality = 0, float O2 = 0,
        float nh3 = 0, float co = 0, float no2 = 0, float c3h8 = 0, float c4h10 = 0, float ch4 = 0, float h2 = 0, float c2h5oh = 0
    ); // Interface reserved parameters

private:
    Epd &_epd;
    UBYTE _buffer[EPD_WIDTH * EPD_HEIGHT / 8];
    
    void drawChar(int x, int y, char c);
    void newlineHandler(int &x, int &y);
};

#endif