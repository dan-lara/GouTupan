#include <Arduino.h>
#include "SHT3xSensor.hpp"
#define TEST_MODE 1

SHT3xSensor sensor; // 使用默认地址0x44和Wire

void setup() 
{
    #if TEST_MODE
        Serial.begin(115200);
    #endif
    
    while (!sensor.begin()) 
    {
        #if TEST_MODE
            Serial.println("SHT3x not found!");
        #endif
        delay(1000);
    }
}

void loop() 
{
    if (sensor.measure()) 
    {
      
        Serial.print("Temperature: ");
        Serial.print(SHT3xsensor.temperature(), 1);
        Serial.print(" *C\tHumidity: ");
        Serial.print(SHT3xsensor.humidity(), 3);
        Serial.println(" %RH");
    } 
    else 
    {
        Serial.println("SHT3x read error");
    }
    delay(1000);
}