// Test_Bench_Transmission_Smart_Farm.ino

#include "LoRa_Transmission.hpp"
#include <ArduinoLowPower.h> //Break low power mode

bool online_mode = 0;

void setup() 
{
    //Simulation mode (off line)

    //Real mode (online)
    //initializeLoRa();
}

void loop() 
{
    // Exemple d envoi de temperature (environ 38.9°C)
    float temperature = 38.924;

    // Appeler la fonction pour envoyer la temperature compressee
      //Simulation mode (off line)
      compressTemperature (temperature); //uint8_t compressTemperature(float temperature);

      //Real mode (online)
      //sendLoRaMessage(temperature);

    for (float ii = 1; ii < 10; ii+=0.3) 
    {
        //Simulation mode (off line)
        compressTemperature (ii + 20);

        //Real mode (online)
        //sendLoRaMessage(ii + 20);

        //LowPower.sleep(20000);  // Envoie le microcontrôleur en mode sommeil pendant 20 secondes BUT NEED Wake up module or RTC timer
        delay(2000);  // Respecter les contraintes de transmission 2s
    }
}
