
#include <Arduino.h>
#include "DFRobot_SEN0308.hpp"

/**
 * @brief Function to initialize the Gravity Soil Moisture Sensor
 * @param analog_pin : GPIO analog pin
 * @return true if a valid value is available. false otherwise
 */
bool SoilMoistureSensor_SEN0308::begin(const uint8_t analog_pin)
{
    _analog_pin = analog_pin;

    return read(false) > 0;
}

/**
 * @brief Function to read the soil moisture value
 * @return The moisture value. A valid reading is between 1 and 3500
 */
uint16_t SoilMoistureSensor_SEN0308::read(bool percentage)
{
    const uint16_t samples = 5; // Fixed number of samples
    const uint16_t delay_ms = 1; // Fixed delay in milliseconds between each sample reading
    uint64_t accumulator = 0;

    for (uint16_t i = 0; i < samples; i++) {
        delay(delay_ms);
        accumulator += analogRead(_analog_pin);
    }

    float value = float(accumulator) / samples;

    if (value > air_humidity)
      value = air_humidity;
    else if (value < water_humidity)
      value = water_humidity;
    // Map the value to a percentage with decimal precision
    float moisturePercent = map(value, air_humidity, water_humidity, 0.0, 100.0);

    // Print the values
    #if TEST_MODE
        Serial.print("Soil Moisture Raw Value: ");
        Serial.print(value);
        Serial.print(" | Moisture: ");
        Serial.print(moisturePercent);
        Serial.println("%");
    #endif

    // Return the raw value for further processing if needed
    return percentage ? moisturePercent : value;
}
