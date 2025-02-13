// Test pour obtenir firmware Version du module : ARD-078 1.2.3

#include <MKRWAN.h>

LoRaModem modem;

void setup() 
{
  Serial.begin(115200);
  while (!Serial);

  if (!modem.begin(EU868)) 
  {  // Choisir la bande régionale correcte (EU868 ici)
    Serial.println("Échec de démarrage du module");
    while (1);
  }

  // Affiche la version du firmware du module LoRa
  Serial.print("Version du module : ");
  Serial.println(modem.version());  // Affiche la version du firmware

}

void loop() 
{
  // NOP
}