// Test_Bench_Transmission_Smart_Farm.ino

#include "LoRa_Transmission.hpp"
#include <ArduinoLowPower.h> // Break low power mode

void setup() 
{
    initializeLoRa();
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

    for (int ii = 0; ii < 100; ii++) 
    {
        #if TEST_MODE
            // Simulation mode (offline)
            compress_2_HEX(((ii / 10) + 20) * 5);
            compress_3_HEX((ii / 10) * 11);
            delay(10000); // 1ooo = 1s => 10ooo = 10s
        #else
            bool Failure_Check = retrySendingStoredPayload(); // Check for sending failure
            // Real mode (online)
            if (Failure_Check)
            {
                Send_LoRa_Data(mux_code, outside_temperature, outside_CO2, outside_humidity, battery_level,
                           soil_nutrients_N_Nitrogen, soil_nutrients_P_Phosphorus, soil_nutrients_K_Potassium,
                           surface_temperature, surface_humidity,
                           deep_temperature, deep_humidity,
                           light_intensity);
            }
            // LowPower.sleep(20000);  // Put the microcontroller into sleep mode for 20 seconds BUT NEEDS a wake-up module or RTC timer
            delay(10000);  // Comply with transmission constraints (200,000 ms = 3.33 min)
        #endif

        
    }
}
