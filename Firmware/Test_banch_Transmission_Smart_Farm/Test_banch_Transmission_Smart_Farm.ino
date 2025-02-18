// Test_Bench_Transmission_Smart_Farm.ino

#include "LoRa_Transmission.hpp"
#include <ArduinoLowPower.h> //Break low power mode

bool online_mode = 1; //TTN Network is valable or not ? 

void setup() 
{
    if (online_mode == 0 ) 
    {
    //Simulation mode (off line)
    }
    else
    {
    initializeLoRa(); //Real mode (online)
    }  
}

void loop() 
{
    // Exemple d envoi de temperature (environ 38.9°C)
    uint8_t mux_code = 0x1; //Sending 1 data
    float outside_temperature = 11.58; //°C
    float outside_humidity = 49.85423; //%
    float outside_CO2 = 3220.53; //ppm

    float battery_level = 82.092; //%
    float soil_nutrients_N_Nitrogen = 198.29; //ppm
    float soil_nutrients_P_Phosphorus = 12.02; //ppm
    float soil_nutrients_K_Potassium = 287.99; //ppm

    float surface_temperature = 8.58; //°C
    float surface_humidity = 78.2723; //%

    float deep_temperature = 7.22; //°C
    float deep_humidity = 87.0793; //%

    for (float ii = 0; ii < 10; ii+=0.1) 
    {
        if (online_mode == 0 ) 
            {
            //Simulation mode (off line)
            compress_2_HEX ((ii + 20)*5);
            compress_3_HEX (ii*11);
            }
        else
            {
            //Real mode (online)
            Send_LoRa_Data(mux_code, 20 + ii, outside_CO2, outside_humidity, battery_level,
            soil_nutrients_N_Nitrogen, soil_nutrients_P_Phosphorus, soil_nutrients_K_Potassium,
            surface_temperature, surface_humidity,
            deep_temperature, deep_humidity
            );
            }
        //LowPower.sleep(20000);  // Envoie le microcontrôleur en mode sommeil pendant 20 secondes BUT NEED Wake up module or RTC timer
        delay(20000);  // Respecter les contraintes de transmission 2s
    }
}
