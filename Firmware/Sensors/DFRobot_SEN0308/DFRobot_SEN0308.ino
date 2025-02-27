#include <Wire.h>
#include "DFRobot_SEN0308.hpp"

#define SENSOR_PIN A1
#define SENSOR_PIN2 A2

SoilMoistureSensor_SEN0308 cap;
SoilMoistureSensor_SEN0308 cap2;

void setup() {
    Serial.begin(9600);
    while (!Serial);

    if (cap.begin(SENSOR_PIN)) {
        Serial.println("Sensor initialized successfully.");
    } else {
        Serial.println("Failed to initialize sensor.");
        while (1);
    }
    if (cap2.begin(SENSOR_PIN2)) {
        Serial.println("Sensor initialized successfully.");
    } else {
        Serial.println("Failed to initialize sensor.");
        while (1);
    }
}

void loop() {

    Serial.print("Sensor A1: ");
    float value = cap.read();
    Serial.print("Sensor A2: ");
    value = cap2.read();
    delay(1000);
}