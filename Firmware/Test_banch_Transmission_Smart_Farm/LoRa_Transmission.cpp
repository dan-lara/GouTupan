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
uint8_t compress_2_HEX (float value) 
{
    // Round the value to the nearest integer
    float truncatedValue = round(value);

    // Convert to an integer (remove decimal part)
    uint16_t intValue = (uint16_t)truncatedValue;

    // Convert to uint8_t (8 bits) with saturation if exceeding the limit
    uint8_t compressedValue = (uint8_t)min(intValue, 255);

    // Debug: Print the compression process
    Serial.print("Original value: ");
    Serial.print(value);
    Serial.print(" -> Rounded: ");
    Serial.print(intValue);
    Serial.print(" -> Compressed: 0x");
    if (compressedValue < 0x10) Serial.print("0"); // Ensure 2-character HEX format
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

void Send_LoRa_Data(
    uint8_t mux_code, 
    float outside_temperature, float outside_CO2, float outside_humidity, 
    float battery_level, 
    float soil_nutrients_N_Nitrogen, float soil_nutrients_P_Phosphorus, float soil_nutrients_K_Potassium, 
    float surface_temperature, float surface_humidity, 
    float deep_temperature, float deep_humidity
)
{
    // 0️⃣ Data Compression Precision
    Serial.println("⚙️ Data Compression Precision : ");
    Serial.println("Outside Temperature ±0.1°C  |  Outside CO2 ±0.5 ppm  |  Outside Humidity ±0.015%");
    Serial.println("Battery Level ±0.2%");
    Serial.println("Surface Temperature ±0.015°C  |  Surface Humidity ±0.015%  |  NPK ±0.5 ppm");
    Serial.println("Deep Temperature ±0.015°C  |  Deep Humidity ±0.015%");
    Serial.println("Lux Sensor ±1.5 lux");
    Serial.println();  // Empty line for better readability

    uint8_t payload[51] = {0};  // Initialize payload array

    // 1️⃣ Add `mux_code` (1 byte)
    payload[0] = mux_code << 4;

    // 2️⃣ 1_HEX byte temperature compression
    uint8_t compressed_outside_temperature = compress_2_HEX(outside_temperature * 5);
    payload[0] += (compressed_outside_temperature >> 4);  // 4 high-order bits
    payload[1] = ((compressed_outside_temperature << 4)); // 4 low-order bits

    // 3️⃣ 1.5_HEX byte CO2 compression
    uint16_t compressed_outside_CO2 = compress_3_HEX(outside_CO2);
    payload[1] += (compressed_outside_CO2) >> 8;  // 4 high-order bits
    payload[2] =  (compressed_outside_CO2);       // 8 low-order bits

    // 4️⃣ 1.5_HEX byte CO2 compression
    uint16_t compressed_outside_humidity = compress_3_HEX(outside_humidity*25); // *10 *2,5 for +/-0.015 
    payload[3] += (compressed_outside_humidity) >> 4;  // 8 high-order bits
    payload[4] =  (compressed_outside_humidity) << 8;  // 4 low-order bits

    // 5️⃣ 1_HEX byte temperature compression
    uint8_t compressed_battery_level = compress_2_HEX(battery_level * 10 / 4 );
    payload[4] += (compressed_battery_level) >> 4;  // 4 high-order bits
    payload[5] =  (compressed_battery_level) << 4;  // 4 low-order bits

    // 6️⃣ Fill the rest of the payload with 0x00 (padding up to 51 bytes)
    for (int i = 6; i < 51; i++) 
    {
        payload[i] = 0x00;
    }

    // 7️⃣ Debug print payload 9️⃣
    Serial.println();  // Empty line for better readability
    Serial.println();  // Empty line for better readability
    Serial.print("Sending LoRa -> Payload: ");
    for (int i = 0; i < 51; i++) 
    {
        if (payload[i] < 0x10) Serial.print("0");  // Format hex on 2 characters
        Serial.print(payload[i], HEX);
        Serial.print(" ");
    }

    int err = 0;  // Initialize error flag

    // 8️⃣ Keep trying until message is sent successfully
    do 
    {
        Serial.println("\nAttempting to send LoRa message...");

        MKR1013modem.beginPacket();
        MKR1013modem.write(payload, sizeof(payload));
        err = MKR1013modem.endPacket(true);  // ❌ FIXED: Removed extra 'int'

        if (err > 0) 
        {
            Serial.println("✅ Message sent successfully!");
        } 
        else 
        {
            Serial.println("❌ Error while sending, PLEASE check antenna, enough Monsieur Inaf !");
            delay(5000);  // Wait 5 seconds before retrying
        }
    } while (err <= 0);  // Retry until err > 0
}

void LoraUnitShipment (float value)  // For debuging
{
    // Compress the temperature before sending
    uint8_t compressedTemp = compress_2_HEX(value);

    int err = 0; // Initialize error flag

    // Keep trying until message is sent successfully
    do 
    {
        Serial.println("Attempting to send LoRa message...");

        // Start LoRa packet
        MKR1013modem.beginPacket();
        MKR1013modem.write(compressedTemp);
        err = MKR1013modem.endPacket(true);

        if (err > 0) 
        {
            Serial.println("✅ Sending successful !");
        } 
        else 
        {
            Serial.println("⚠️ Error, check Antenna PLEASE ! Retrying ...");
            delay(5000);  // Wait 5 seconds before retrying (adjust if needed)
        }
    } while (err <= 0); // Retry until err > 0
}
