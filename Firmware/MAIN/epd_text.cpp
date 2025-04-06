#include "epd_text.hpp"
#include <avr/pgmspace.h>

#ifndef HAS_DTOSTRF
char* dtostrf(double val, signed char width, unsigned char prec, char* buf) {
    sprintf(buf, "%*.*f", width, prec, val);
    return buf;
}
#endif

#define TEST_MODE 0
// According to imagedata.cpp analysis, the screen resolution is 128x296, and the buffer length = 128/8*296 = 4736 bytes
#define BUFFER_SIZE 4736 
// #define TEST_MODE 1

EpdText::EpdText(Epd &epd) : _epd(epd) 
{
    memset(_buffer, 0xFF, BUFFER_SIZE); // Initialized to full white (0xFF represents white)
}

void EpdText::init() 
{
    _epd.Init();
}

void EpdText::clear() 
{
    memset(_buffer, 0xFF, BUFFER_SIZE); // Clear screen: Fill with all white
    _epd.DisplayFrame(_buffer, _buffer); // Double buffer (black white, red) uses the same data
    _epd.WaitUntilIdle();  // Ensure that the screen is cleared successfully
}

void EpdText::drawChar(int x, int y, char c) 
{
    #if TEST_MODE
        Serial.print("Drawing char: ");
        Serial.println(c);
    #endif

    // Coordinate conversion: x along the long side (296px direction), y along the wide side (128px direction)
    if (x < 0 || x >= EPD_HEIGHT || y < 0 || y >= EPD_WIDTH) return;

    // Font index calculation (0x21 corresponds to ASCII 33 '!')
    uint8_t font_index = static_cast<uint8_t>(c) - 0x21;
    if (font_index >= 224) return;

    for (int j = 0; j < CHAR_HEIGHT; j++) {
        uint8_t line = pgm_read_byte(&Font8x8_FR[font_index][j]);
        for (int i = 0; i < CHAR_WIDTH; i++) {
            if (line & (0x80 >> i)) {
                // Coordinate calculation (adapting to the physical layout of the screen)
                int px = x + j; // Long side direction
                int py = EPD_WIDTH - 1 - (y + i); // Mirror image in broadside direction
                
                // Calculate buffer position (each byte represents 8 vertical pixels)
                if (py >= 0 && py < EPD_WIDTH && px < EPD_HEIGHT) {
                    int byteIndex = (py / 8) + (px * (EPD_WIDTH / 8));
                    uint8_t bitMask = 0x80 >> (py % 8);
                    _buffer[byteIndex] &= ~bitMask; // Set black (0 means black)
                }
            }
        }
    }
}

void EpdText::newlineHandler(int &x, int &y) 
{
    y += CHAR_HEIGHT + LINE_SPACING;
    x = 0;
    if (y + CHAR_HEIGHT > EPD_WIDTH) 
    { // Dealing with bottoming out
        y = 0;
        //clear(); // Clear screen and reset
    }
}

void EpdText::displayText(const char* text) 
{
    int currentX = 0; // Along the long side (296px direction)
    int currentY = 0; // Along the width (128px)

    #if TEST_MODE
        Serial.println(">>> TEXT TO DISPLAY:");
        Serial.println(text);  // Print the content to be displayed
        Serial.println();
    #endif

    while (*text) 
    {
        if (*text == '\n') 
        {
            newlineHandler(currentX, currentY);
            text++;
            continue;
        }

        drawChar(currentX, currentY, *text);
        currentX += CHAR_WIDTH;

        if (currentX + CHAR_WIDTH > EPD_HEIGHT) 
        { // Long edge line break
            newlineHandler(currentX, currentY);
        }

        text++;
    }

    _epd.DisplayFrame(_buffer, _buffer);
    _epd.WaitUntilIdle();  // Ensure that the screen is cleared successfully
}

void EpdText::updateDisplay
(
    float mux_code, float outside_temperature, float outside_CO2, float outside_humidity, float battery_level,
    float soil_nutrients_N_Nitrogen, float soil_nutrients_P_Phosphorus, float soil_nutrients_K_Potassium,
    float surface_temperature, float surface_humidity,
    float deep_temperature, float deep_humidity,
    float light_intensity, float light_infrared, float light_ultraviolet,
    float R_RGB, float G_RGB, float B_RGB,
    float pressure, float quality, float O2,
    float nh3, float co, float no2, float c3h8, float c4h10, float ch4, float h2, float c2h5oh
) 
{
    if (_epd.eink_failed) {
        #if TEST_MODE
            Serial.println("E-ink failed, skipping display.");
        #endif
        return;
    }

    String text;
    text.reserve(1024);  // Preallocate memory

    // Splicing data (standard formatting)
    // text += "T:" + String(outside_temperature, 1) + "C, ";
    // text += "H:" + String(outside_humidity, 1) + "%, ";
    // text += "CO2:" + String(outside_CO2, 0) + "ppm,\n";
    
    // text += "NPK:" + String(soil_nutrients_N_Nitrogen, 1) + ", ";
    // text += String(soil_nutrients_P_Phosphorus, 1) + ", ";
    // text += String(soil_nutrients_K_Potassium, 1) + ",\n";
    
    // text += "Surface T/H:" + String(surface_temperature, 1) + ", " + String(surface_humidity, 1) + ",\n";
    // text += "Deep T/H:" + String(deep_temperature, 1) + ", " + String(deep_humidity, 1) + ",\n";
    
    // text += "Light:" + String(light_intensity, 1) + ", IR:" + String(light_infrared, 1);
    // text += ", UV:" + String(light_ultraviolet, 1) + ",\n";
    
    // text += "RGB:" + String(R_RGB, 0) + ", " + String(G_RGB, 0) + ", " + String(B_RGB, 0) + ",\n";
    
    // text += "P:" + String(pressure, 1) + "hPa, ";
    // text += "O2:" + String(O2, 1) + ", Q:" + String(quality, 1) + ",\n";
    
    // text += "Gas: NH3:" + String(nh3, 2) + ", CO:" + String(co, 2) + ", NO2:" + String(no2, 2) + ",\n";
    // text += "CH4:" + String(ch4, 2) + ", C3H8:" + String(c3h8, 2) + ", C4H10:" + String(c4h10, 2) + ",\n";
    // text += "H2:" + String(h2, 2) + ", EtOH:" + String(c2h5oh, 2);

    // text += ".....................................\n";
    text += "            Projet GouTupan\nAcess: grafana.danlara.com.br\n\n";

    text += "Air: T/H "+String(outside_temperature, 1) + "C, ";
    text += String(outside_humidity, 1) + "%, ";
    text += "CO2: "+String(outside_CO2, 0) + "ppm,\n";

    text += "Surface 5cm T/H : "+String(surface_temperature, 1) + "C, " + String(surface_humidity, 1) + "%,\n";
    text += "Deep 45cm T/H : "+String(deep_temperature, 1) + "C, " + String(deep_humidity, 1) + "%,\n";

    text += "\nN: "+String(soil_nutrients_N_Nitrogen, 1) + "ppm, ";
    text += "P: "+String(soil_nutrients_P_Phosphorus, 1) + "ppm, ";
    text += "K: "+String(soil_nutrients_K_Potassium, 1) + "ppm,\n";

    text += "Light: "+String(light_intensity, 1) + ", IR: " + String(light_infrared, 1) + ", UV: " + String(light_ultraviolet, 1) + ",\n";

    text += ".....................................\n";
    text += "Instale par EI4 en 31/03/2025\nDaniel, Vladislav, Xiaochen, Yulin";


    // Clear the buffer and display
    memset(_buffer, 0xFF, BUFFER_SIZE);
    displayText(text.c_str());  // Pass the correct string directly
    
    #if TEST_MODE
        Serial.print("Display updated, wait for idle...\n");
    #endif
}
