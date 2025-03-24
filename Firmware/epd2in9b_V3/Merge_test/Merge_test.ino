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
#include "SHT3xSensor.hpp"

#define TEST_MODE 1
#define OPERATION_MODE 0
#define NB_MAX_SENSOR_ATTEMPT 10

Epd epd;
EpdText epdText(epd);
SHT3xSensor SHT3xsensor(0x44, &Wire);// ADDR: 0 => 0x44

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(2000);

  #if TEST_MODE
  Serial.print("2.9inch b V3 e-Paper init \r\n ");
  Serial.print("2.9inch b V3 e-Paper init \r\n ");
  Serial.print("2.9inch b V3 e-Paper init \r\n ");
  #endif

  if (epd.Init() != 0) {
      #if TEST_MODE
        Serial.print("e-Paper init failed\r\n ");
      #endif
      return;
  }
  
    #if TEST_MODE
      Serial.print("e-Paper Clear\r\n ");
    #endif
    epd.Clear();

  
    Serial.println("Try to boot SHT3x");
    int attempts = 0;
    while (!SHT3xsensor.begin() && attempts < NB_MAX_SENSOR_ATTEMPT) 
    {
        Serial.println("Into the loop");
        #if TEST_MODE
            Serial.println("ERROR: SHT3x sensor not detected!");
        #endif
        delay(400); 
        attempts++;
    }
}

void loop() {
  // put your main code here, to run repeatedly:
  auto data_SHT3xsensor = SHT3xsensor.measure();
  float deep_temperature = data_SHT3xsensor.first;
  float deep_humidity = data_SHT3xsensor.second;
  
  #if TEST_MODE
    Serial.print("Temperature: ");
    Serial.print(deep_temperature, 1);
    Serial.print(" *C\tHumidity: ");
    Serial.print(deep_humidity, 1);
    Serial.print(" %RH");
    Serial.println();
  
    Serial.println("Start to print on the E-ink");
  #endif
  epdText.updateDisplay(0, 23.4, 450.0, 40.2, 85.6,
                      10, 5, 12,
                      25.0, 45.0,
                      deep_temperature, deep_humidity,
                      600, 100, 30,
                      255, 128, 64,
                      1013, 80, 20.9,
                      0.1, 2.3, 0.5, 0.7, 0.6, 1.0, 0.2, 0.3);
  #if TEST_MODE
    Serial.println("Printed on the E-ink");
  #endif
  
  delay(1000);
  
}
