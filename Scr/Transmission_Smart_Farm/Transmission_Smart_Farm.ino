//Test antenne

#include <MKRWAN.h>  // Bibliothèque pour gérer le module LoRa intégré

LoRaModem MKR1013modem;

#include "arduino_secrets.h"
String appEui = "AA2025AAAA2025AA";
String appKey = "7E72F8E351AA2605A8F71F03CE6835DC";

void setup() 
{
  Serial.begin(115200);
  while (!Serial);

  Serial.println("\nDémarrage du module LoRa...");
  
  // Initialisation du module LoRa avec la fréquence EU868
  if (!MKR1013modem.begin(EU868)) 
  {
    Serial.println("Échec de démarrage du module LoRa !");
    while (1);
  }

  // Afficher les infos du module
  Serial.print("Version du firmware : ");
  Serial.println(MKR1013modem.version());

  Serial.print("Device EUI : ");
  Serial.println(MKR1013modem.deviceEUI());

  // Connexion au réseau LoRaWAN en OTAA
  Serial.println("Connexion au réseau TTN...");
  int connected = MKR1013modem.joinOTAA(appEui, appKey);

  if (!connected) 
  {
    Serial.println("Échec de connexion ! Vérifiez votre signal et réessayez.");
    while (1);
  }

  Serial.println("Connecté à TTN !");
  
  // Configuration du temps minimum entre deux envois (60s ici)
  MKR1013modem.minPollInterval(60);
}

void loop() 
{
  Serial.println();
  Serial.println("Début de transmission...");

  for (int ii = 1; ii < 10; ii++) 
  {
    Serial.print("Envoi du message : ");
    Serial.println(ii + 20);

    // Préparation du paquet LoRa
    MKR1013modem.beginPacket();
    MKR1013modem.print(ii + 20);
    int err = MKR1013modem.endPacket(true);

    if (err > 0) 
    {
      Serial.println("Message envoyé avec succès !");
    } else 
    {
      Serial.println("Erreur lors de l'envoi du message.");
    }

    delay(60000);  // Respecter les contraintes de transmission (min 1 message/minute)
  }
}