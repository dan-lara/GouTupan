#include <Wire.h>
#include "DFRobot_SEN0308.hpp"

#define SENSOR_PIN A1

SoilMoistureSensor_SEN0308 cap;

void setup() {
    Serial.begin(9600);
    while (!Serial);

    if (cap.begin(SENSOR_PIN)) 
    {
        Serial.println("Sensor initialized successfully.");
    } 
  
    else 
    {
        Serial.println("Failed to initialize sensor.");
        int i = 0  
      while (i<7)
        {
        i++;
        delay(500);
        }
    }
}

void loop() 
{
    Serial.print("Sensor A1: ");
    float value = cap.read();
    delay(1000);
}