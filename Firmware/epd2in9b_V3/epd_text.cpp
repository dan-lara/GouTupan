#include "epd_text.hpp"
#include <avr/pgmspace.h>

#ifndef HAS_DTOSTRF
char* dtostrf(double val, signed char width, unsigned char prec, char* buf) {
    sprintf(buf, "%*.*f", width, prec, val);
    return buf;
}
#endif

#define TEST_MODE 1
// 根据imagedata.cpp分析，屏幕分辨率为128x296，缓冲区长=128/8*296=4736字节
#define BUFFER_SIZE 4736 
// #define TEST_MODE 1

EpdText::EpdText(Epd &epd) : _epd(epd) {
    memset(_buffer, 0xFF, BUFFER_SIZE); // 初始化为全白（0xFF代表白色）
}

void EpdText::init() {
    _epd.Init();
}

void EpdText::clear() {
    memset(_buffer, 0xFF, BUFFER_SIZE); // 清屏：全白填充
    _epd.DisplayFrame(_buffer, _buffer); // 双缓冲都用同一数据
    _epd.WaitUntilIdle();  // 添加这一行，确保清屏成功
}

void EpdText::drawChar(int x, int y, char c) {
    #if TEST_MODE
        Serial.print("Drawing char: ");
        Serial.println(c);
    #endif

    // 坐标转换：x沿长边（296px方向），y沿宽边（128px方向）
    if (x < 0 || x >= EPD_HEIGHT || y < 0 || y >= EPD_WIDTH) return;

    // 字体索引计算（0x21对应ASCII 33 '!'）
    uint8_t font_index = static_cast<uint8_t>(c) - 0x21;
    if (font_index >= 224) return;

    for (int j = 0; j < CHAR_HEIGHT; j++) {
        uint8_t line = pgm_read_byte(&Font8x8_FR[font_index][j]);
        for (int i = 0; i < CHAR_WIDTH; i++) {
            if (line & (0x80 >> i)) {
                // 坐标计算（适配屏幕物理布局）
                int px = x + j; // 长边方向
                int py = EPD_WIDTH - 1 - (y + i); // 宽边方向镜像（可选，根据实际显示方向调整）
                
                // 计算缓冲区位置（每个字节代表8个垂直像素）
                if (py >= 0 && py < EPD_WIDTH && px < EPD_HEIGHT) {
                    int byteIndex = (py / 8) + (px * (EPD_WIDTH / 8));
                    uint8_t bitMask = 0x80 >> (py % 8);
                    _buffer[byteIndex] &= ~bitMask; // 设置黑色（0代表黑色）
                }
            }
        }
    }
}

void EpdText::newlineHandler(int &x, int &y) {
    y += CHAR_HEIGHT + LINE_SPACING;
    x = 0;
    if (y + CHAR_HEIGHT > EPD_WIDTH) { // 触底处理
        y = 0;
        //clear(); // 清屏并重置
    }
}

void EpdText::displayText(const char* text) {
    int currentX = 0; // 沿长边（296px方向）
    int currentY = 0; // 沿宽边（128px方向）

    #if TEST_MODE
        Serial.println(">>> TEXT TO DISPLAY:");
        Serial.println(text);  // 打印出你真正准备显示的内容
        Serial.println();
    #endif

    while (*text) {
        if (*text == '\n') {
            newlineHandler(currentX, currentY);
            text++;
            continue;
        }

        drawChar(currentX, currentY, *text);
        currentX += CHAR_WIDTH;

        if (currentX + CHAR_WIDTH > EPD_HEIGHT) { // 长边越界换行
            newlineHandler(currentX, currentY);
        }

        text++;
    }

    _epd.DisplayFrame(_buffer, _buffer);
    _epd.WaitUntilIdle();  // 添加这一行，确保清屏成功
}

/*
void EpdText::updateDisplay(float Tair, float T10, float T30, float Hair, float H10, float H30, float pressure) {
    char buf[120];
    snprintf(buf, sizeof(buf), 
        "Bonjour !\n"
        "Tair:%.1fC T10:%.1fC T30:%.1fC\n"
        "Hair:%.1f H10:%.1f H30:%.1f\n"
        "Pressure:%.1f hpa",
        Tair, T10, T30, Hair, H10, H30, pressure
    );
    
    memset(_buffer, 0xFF, BUFFER_SIZE); // 生成新数据前清空为全白
    displayText(buf);
    
    Serial.print("Display updated, wait for idle...\n");
    _epd.WaitUntilIdle();
}
*/

void EpdText::updateDisplay(
    float mux_code, float outside_temperature, float outside_CO2, float outside_humidity, float battery_level,
    float soil_nutrients_N_Nitrogen, float soil_nutrients_P_Phosphorus, float soil_nutrients_K_Potassium,
    float surface_temperature, float surface_humidity,
    float deep_temperature, float deep_humidity,
    float light_intensity, float light_infrared, float light_ultraviolet,
    float R_RGB, float G_RGB, float B_RGB,
    float pressure, float quality, float O2,
    float nh3, float co, float no2, float c3h8, float c4h10, float ch4, float h2, float c2h5oh
) {
    String text;
    text.reserve(1024);  // 预分配内存

    // 拼接数据（规范格式化）
    text += "T:" + String(outside_temperature, 1) + "C, ";
    text += "H:" + String(outside_humidity, 1) + "%, ";
    text += "CO2:" + String(outside_CO2, 0) + "ppm,\n";
    
    text += "NPK:" + String(soil_nutrients_N_Nitrogen, 1) + ", ";
    text += String(soil_nutrients_P_Phosphorus, 1) + ", ";
    text += String(soil_nutrients_K_Potassium, 1) + ",\n";
    
    text += "Surface T/H:" + String(surface_temperature, 1) + ", " + String(surface_humidity, 1) + ",\n";
    text += "Deep T/H:" + String(deep_temperature, 1) + ", " + String(deep_humidity, 1) + ",\n";
    
    text += "Light:" + String(light_intensity, 1) + ", IR:" + String(light_infrared, 1);
    text += ", UV:" + String(light_ultraviolet, 1) + ",\n";
    
    text += "RGB:" + String(R_RGB, 0) + ", " + String(G_RGB, 0) + ", " + String(B_RGB, 0) + ",\n";
    
    text += "P:" + String(pressure, 1) + "hPa, ";
    text += "O2:" + String(O2, 1) + ", Q:" + String(quality, 1) + ",\n";
    
    text += "Gas: NH3:" + String(nh3, 2) + ", CO:" + String(co, 2) + ", NO2:" + String(no2, 2) + ",\n";
    text += "CH4:" + String(ch4, 2) + ", C3H8:" + String(c3h8, 2) + ", C4H10:" + String(c4h10, 2) + ",\n";
    text += "H2:" + String(h2, 2) + ", EtOH:" + String(c2h5oh, 2);

    // 清空缓冲区并显示
    memset(_buffer, 0xFF, BUFFER_SIZE);
    displayText(text.c_str());  // 直接传递正确字符串
    
    #if TEST_MODE
        Serial.print("Display updated, wait for idle...\n");
    #endif
}
