// Test_Bench_Transmission_Smart_Farm.ino

#include "LoRa_Transmission.hpp"

void setup() 
{
    initializeLoRa();
}

void loop() 
{
    for (int ii = 1; ii < 10; ii++) 
    {
        sendLoRaMessage(ii + 20);
        delay(60000);  // Respecter les contraintes de transmission (min 1 message/minute)
    }
}
