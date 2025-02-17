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
    float truncatedTemp = round(temperature);

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

// Fonction pour compresser la température
uint16_t compress_3_HEX(float no_compressed) 
{
    // Round data
    uint16_t truncatedTemp = round(no_compressed);

    // Affichage dans le Serial Monitor
    Serial.print("Original data : ");
    Serial.print(no_compressed);
    Serial.print(" -> Compressed : 0x");
    if (truncatedTemp < 0x10) Serial.print("0"); // Add "O" for have 2nd char
    if (truncatedTemp < 0x100) Serial.print("0"); // Add "O" for have 3rd char
    Serial.println(truncatedTemp, HEX);

    return truncatedTemp;
}

void Send_LoRa_Data(uint8_t mux_code, float outside_temperature, float outside_CO2) 
{
    uint8_t payload[51] = {0};  // Initialisation du payload

    // 1️⃣ Ajouter le `mux_code` (1 octet)
    payload[0] = mux_code << 4;

    // 2️⃣ 1_HEX byte temperature compression
    uint8_t compressed_outside_temperature = compressTemperature (outside_temperature*10);
    payload[0] += (compressed_outside_temperature >> 4);
    payload[1] = ((compressed_outside_temperature << 4));

    // 3️⃣ 1,5_HEX byte CO2 compression
    uint16_t compressed_outside_CO2 = compress_3_HEX (outside_CO2);
    payload[1] += (compressed_outside_CO2) >> 8;  // 4 high-order bits
    payload[2] =  (compressed_outside_CO2);       // 8 low-order bits

    // 4️⃣ Remplir le reste du payload avec 0x00 pour arriver à 51 octets
    
    for (int i = 3; i < 51; i++) 
    {
        payload[i] = 0x00;  // Placeholder
    }

    // 5️⃣ Affichage Debug
    Serial.print("Envoi LoRa -> Payload : ");
    for (int i = 0; i < 51; i++) 
    {
        if (payload[i] < 0x10) Serial.print("0");  // Format hex sur 2 caractères
        Serial.print(payload[i], HEX);
        Serial.print(" ");
    }
    Serial.println();

    // 6️⃣ Envoi via LoRa
    MKR1013modem.beginPacket();
    MKR1013modem.write(payload, sizeof(payload));
    int err = MKR1013modem.endPacket(true);

    if (err > 0) 
    {
        Serial.println("✅ Message envoyé avec succès !");
    } else 
    {
        Serial.println("❌ Error while sending, PLEASE check antenna, enough Monsieur Inaf !");
    }
}

void sendLoRaMessage(float value) 
{
    // Compresser la température avant l'envoi
    uint8_t compressedTemp = compressTemperature(value);

    // Envoi du message LoRa en binaire
    MKR1013modem.beginPacket();
    MKR1013modem.write(compressedTemp);  // Envoie un vrai byte hexadécimal
    int err = MKR1013modem.endPacket(true);

    if (err > 0) 
    {
        Serial.println("Sending successfull !");
    } else 
    {
        Serial.println("Error, check Antenna PLEASE !!!");
    }
}
