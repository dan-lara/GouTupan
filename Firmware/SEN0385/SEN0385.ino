#include <Arduino.h>
#include "SHT3xSensor.hpp"

SHT3xSensor sensor; // 使用默认地址0x44和Wire

void setup() {
    Serial.begin(9600);
    
    while (!sensor.begin()) {
        Serial.println("SHT3x not found!");
        delay(1000);
    }
}

void loop() {
    if (sensor.measure()) {
        Serial.print("Temperature: ");
        Serial.print(sensor.temperature(), 1);
        Serial.print(" *C\tHumidity: ");
        Serial.print(sensor.humidity(), 1);
        Serial.println(" %RH");
    } else {
        Serial.println("SHT3x read error");
    }
    delay(1000);
}