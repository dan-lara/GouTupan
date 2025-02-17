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

// Fonction pour compresser la température
uint8_t compressTemperature(float temperature) 
{
    // Arrondir la température à 1 décimale
    float truncatedTemp = round(temperature * 10);

    // Convertir la température en entier, en supprimant la partie décimale
    uint16_t tempInCelsius = (uint16_t)truncatedTemp;

    // Diviser la valeur par 2
    uint8_t halfTemp = tempInCelsius / 2;

    // Convertir en uint8_t (8 bits)
    uint8_t compressedValue = (uint8_t)halfTemp;

    // Affichage dans le Serial Monitor
    Serial.print("Température originale : ");
    Serial.print(temperature);
    Serial.print("°C -> Arrondi/2 : ");
    Serial.print(halfTemp);
    Serial.print(" -> Comprimé : 0x");
    if (compressedValue < 0x10) Serial.print("0"); // Ajoute un '0' pour avoir 2 caractères
    Serial.println(compressedValue, HEX);

    return compressedValue;
}

void sendLoRaMessage(float value) 
{
    // Compresser la température avant l'envoi
    uint8_t compressedTemp = compressTemperature(value);

    Serial.println();
    Serial.print("Envoi du message (température compressée) : 0x");
    if (compressedTemp < 0x10) Serial.print("0");  // Affichage formaté
    Serial.println(compressedTemp, HEX);

    // Envoi du message LoRa en binaire
    MKR1013modem.beginPacket();
    MKR1013modem.write(compressedTemp);  // Envoie un vrai byte hexadécimal
    int err = MKR1013modem.endPacket(true);

    if (err > 0) 
    {
        Serial.println("Message envoyé avec succès !");
    } else 
    {
        Serial.println("Erreur lors de l'envoi du message.");
    }
}
