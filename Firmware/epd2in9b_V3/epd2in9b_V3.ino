/**
 *  @filename   :   epd2in9b_V3-demo.ino
 *  @brief      :   2.9inch e-paper display demo
 *  @author     :   Waveshare
 *
 *  Copyright (C) Waveshare     Dec 3 2020
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documnetation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to  whom the Software is
 * furished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <SPI.h>
#include "epd2in9b_V3.hpp"
#include "epd_text.hpp"

Epd epd;
EpdText epdText(epd);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(2000);

  if (epd.Init() != 0) {
      Serial.print("e-Paper init failed\r\n ");
      return;
  }
  Serial.print("2.9inch b V3 e-Paper init \r\n ");
  
  // 显示法语文本（带换行）
  display.DisplayText("Bonjour 25\xB0C\nHumidité: 75%", 10, 20);
    
  display.RefreshScreen();  // 刷新屏幕，使文本生效

  delay(1000);
  epd.Clear();
}

void loop() {
  // put your main code here, to run repeatedly:

}