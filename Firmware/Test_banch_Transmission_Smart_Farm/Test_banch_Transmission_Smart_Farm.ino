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
    float temperature = 38.924;
    uint8_t mux_code = 0x1;
    float outside_temperature = 9.58;
    float outside_CO2 = 3220.53;

    for (float ii = 0; ii < 10; ii+=0.1) 
    {
        if (online_mode == 0 ) 
            {
            //Simulation mode (off line)
            compressTemperature (ii + 20);
            compress_3_HEX (ii*11);
            }
        else
            {
            //Real mode (online)
            Send_LoRa_Data(mux_code, 20 + ii, outside_CO2);            
            }

        //LowPower.sleep(20000);  // Envoie le microcontrôleur en mode sommeil pendant 20 secondes BUT NEED Wake up module or RTC timer
        delay(20000);  // Respecter les contraintes de transmission 2s
    }
}
