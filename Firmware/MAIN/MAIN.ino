// Read_and_Send.ino

#include "LoRa_Transmission.hpp"
#include <ArduinoLowPower.h> // Break low power mode
#include "Baroleter_High_Accuracy.hpp"

HP20x barometer;  // Create sensor object

void setup() 
{
    initializeLoRa();

    // Sensors Setup
    barometer.begin();
}

void loop() 
{
    // Example of sending temperature data (approximately 38.9°C)
    uint8_t mux_code = 0x1; // Sending 1 data
    float outside_temperature = -71.5865; // °C
    float outside_humidity = 49.85423; // %
    float outside_CO2 = 3220.53; // ppm

    float battery_level = 82.092; // %
    float soil_nutrients_N_Nitrogen = 198.29; // ppm
    float soil_nutrients_P_Phosphorus = 13.22; // ppm
    float soil_nutrients_K_Potassium = 287.99; // ppm

    float surface_temperature = 8.58; // °C
    float surface_humidity = 78.2723; // %

    float deep_temperature = 7.22; // °C
    float deep_humidity = 87.0793; // %

    float light_intensity = 13929.928; // Lux
    float light_infrared = 928.13; // Lux
    float light_ultraviolet = 1329.58; // Lux

    float R_RGB = 244.34;
    float G_RGB = 35.23;
    float B_RGB = 40.9;

    float pressure = barometer.readPressure();

    #if TEST_MODE
        Serial.print("Pressure: ");
        Serial.print(pressure);
        Serial.println(" hPa");
    #endif

    bool Failure_Check = retrySendingStoredPayload(); // Check for sending failure
    // Real mode (online)
    if (Failure_Check)
    {
        Send_LoRa_Data(mux_code, outside_temperature, outside_CO2, outside_humidity, battery_level,
                    soil_nutrients_N_Nitrogen, soil_nutrients_P_Phosphorus, soil_nutrients_K_Potassium,
                    surface_temperature, surface_humidity,
                    deep_temperature, deep_humidity,
                    light_intensity, light_infrared, light_ultraviolet,
                    R_RGB, G_RGB, B_RGB,
                    pressure);
    }
    // LowPower.sleep(20000);  // Put the microcontroller into sleep mode for 20 seconds BUT NEEDS a wake-up module or RTC timer
    delay(10000);  // Comply with transmission constraints (200,000 ms = 3.33 min)
        
}
