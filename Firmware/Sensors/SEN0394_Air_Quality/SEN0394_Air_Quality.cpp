#include "SEN0394_Air_Quality.hpp"
#include <Wire.h>

AirSensor::AirSensor()
{
    // Constructeur pour initialiser les variables, si besoin
}

bool AirSensor::begin(uint8_t maxAttempts)
{
    return initSensor(maxAttempts);
}

bool AirSensor::initSensor(uint8_t maxAttempts)
{
    uint8_t attempts = 0;
    #if TEST_MODE
        Serial.begin(115200);
    #endif
    while (attempts < maxAttempts)
    {
        if (mySgp40.begin(2000))
        {
          #if TEST_MODE
              Serial.println("SGP40 initialized successfully!");
          #endif
            return true;
        }
        #if TEST_MODE
            Serial.println("Failed to initialize chip. Retrying...");
        #endif
        attempts++;
        delay(1000);
    }
    #if TEST_MODE
        Serial.println("Failed to initialize sensor after max attempts.");
    #endif
    return false;
}

float AirSensor::readAirQuality()
{
    // Lire l'indice de qualité de l'air (VOC)
    uint16_t vocIndex = mySgp40.getVoclndex();
    return static_cast<float>(vocIndex);
}
