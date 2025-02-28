#include "epd_text.h"
#include <avr/pgmspace.h>

EpdText::EpdText(Epd &epd) : _epd(epd) {
    memset(_buffer, 0xFF, sizeof(_buffer)); // 初始化为全白
}

void EpdText::init() {
    _epd.Init();
}

void EpdText::clear() {
    memset(_buffer, 0xFF, sizeof(_buffer));
    _epd.DisplayFrame(_buffer, _buffer);
}

void EpdText::drawChar(int x, int y, char c) {
    // 坐标范围限制
    if (x < 0 || x >= EPD_HEIGHT || y < 0 || y >= EPD_WIDTH) return;

    // 字体索引计算（0x20-0xFF映射到0-223）
    uint8_t font_index = static_cast<uint8_t>(c) - 0x20;
    if (font_index >= 224) return;

    for (int j = 0; j < CHAR_HEIGHT; j++) {
        uint8_t line = pgm_read_byte(&Font8x8_FR[font_index][j]);
        for (int i = 0; i < CHAR_WIDTH; i++) {
            if (line & (0x80 >> i)) {
                // 坐标旋转：文字沿长边（296px）方向显示
                int px = x + j;                // 长边方向（Y轴）
                int py = EPD_WIDTH - 1 - (y + i); // 宽边方向（X轴），从右向左
                
                // 计算缓冲区位置
                int byteIndex = (py / 8) + (px * (EPD_WIDTH / 8));
                uint8_t bitMask = 0x80 >> (py % 8);
                _buffer[byteIndex] &= ~bitMask;
            }
        }
    }
}

void EpdText::newlineHandler(int &x, int &y) {
    x += CHAR_HEIGHT + LINE_SPACING; // 沿长边换行
    y = 0;
    if (x + CHAR_HEIGHT > EPD_HEIGHT) {
        x = 0;  // 触底重置
        clear();
    }
}

void EpdText::displayText(const char* text) {
    static int currentX = 0;  // 沿长边（296px方向）
    static int currentY = 0;  // 沿宽边（128px方向）

    while (*text) {
        if (*text == '\n') {
            newlineHandler(currentX, currentY);
            text++;
            continue;
        }

        drawChar(currentX, currentY, *text);
        currentY += CHAR_WIDTH;

        if (currentY + CHAR_WIDTH > EPD_WIDTH) {
            newlineHandler(currentX, currentY);
        }

        text++;
    }

    _epd.DisplayFrame(_buffer, _buffer);
}

void EpdText::updateDisplay(float temp, float humidity, float pressure) {
    char buf[60];
    snprintf(buf, sizeof(buf), 
        "Bonjour !\n"
        "Temp:%.1f\x80\n"   // \x80对应℃符号
        "Humid:%.1f%%",
        temp, humidity
    );
    
    clear();
    displayText(buf);
}