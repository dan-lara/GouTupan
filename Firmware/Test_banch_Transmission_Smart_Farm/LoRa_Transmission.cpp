#include "LoRa_Transmission.hpp"
#include "Arduino_Key.hpp"  // Contient les constantes APPEUI et APPKEY

LoRaModem MKR1013modem;

void initializeLoRa() 
{
    Serial.begin(115200);
    while (!Serial);

    Serial.println("\nDémarrage du module LoRa...");
  
    if (!MKR1013modem.begin(EU868)) 
    {
        Serial.println("Échec de démarrage du module LoRa !");
        while (1);
    }

    Serial.print("Version du firmware : ");
    Serial.println(MKR1013modem.version());

    Serial.print("Device EUI : ");
    Serial.println(MKR1013modem.deviceEUI());

    Serial.println("Connexion au réseau TTN...");
    int connected = MKR1013modem.joinOTAA(APPEUI, APPKEY);

    if (!connected) 
    {
        Serial.println("Échec de connexion ! Vérifiez votre signal et réessayez.");
        while (1);
    }

    Serial.println("Connecté à TTN !");
    MKR1013modem.minPollInterval(60);
}

void sendLoRaMessage(int value) 
{
    Serial.println();
    Serial.print("Envoi du message : ");
    Serial.println(value);

    MKR1013modem.beginPacket();
    MKR1013modem.print(value);
    int err = MKR1013modem.endPacket(true);

    if (err > 0) 
    {
        Serial.println("Message envoyé avec succès !");
    } else 
    {
        Serial.println("Erreur lors de l'envoi du message.");
    }
}
