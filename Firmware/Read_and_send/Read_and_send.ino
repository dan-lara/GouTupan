#include "LoRa_Transmission.hpp"
#include "Arduino_Key.hpp"  // Contient les constantes APPEUI et APPKEY

LoRaModem MKR1013modem;
bool Test_Mode = 0;  // Set to 1 to enable Serial Monitor debugging, 0 to disable

void initializeLoRa() 
{
    if (Test_Mode) 
    {
        Serial.begin(115200);
        while (!Serial);
        Serial.println("\nStarting LoRa module...");
    }
  
    if (!MKR1013modem.begin(EU868)) 
    {
        if (Test_Mode) Serial.println("LoRa module startup failed!");
        while (1);
    }

    if (Test_Mode) 
    {
        Serial.print("Firmware version: ");
        Serial.println(MKR1013modem.version());

        Serial.print("Device EUI: ");
        Serial.println(MKR1013modem.deviceEUI());

        Serial.println("Connecting to TTN...");
    }

    int connected = MKR1013modem.joinOTAA(APPEUI, APPKEY);

    if (!connected) 
    {
        if (Test_Mode) Serial.println("Connection failed! Check your signal and retry.");
        while (1);
    }

    if (Test_Mode) Serial.println("Connected to TTN!");
    MKR1013modem.minPollInterval(60);
}

uint8_t compress_2_HEX(float value) 
{
    float truncatedValue = round(value);
    uint16_t intValue = (uint16_t)truncatedValue;
    uint8_t compressedValue = (uint8_t)min(intValue, 255);

    if (Test_Mode) 
    {
        Serial.print("Original value: ");
        Serial.print(value);
        Serial.print(" -> Rounded: ");
        Serial.print(intValue);
        Serial.print(" -> Compressed: 0x");
        if (compressedValue < 0x10) Serial.print("0");
        Serial.println(compressedValue, HEX);
    }

    return compressedValue;
}

uint16_t compress_3_HEX(float no_compressed) 
{
    uint16_t truncatedTemp = round(no_compressed);

    if (Test_Mode) 
    {
        Serial.print("Original data: ");
        Serial.print(no_compressed);
        Serial.print(" -> Compressed: 0x");
        if (truncatedTemp < 0x10) Serial.print("0");
        if (truncatedTemp < 0x100) Serial.print("0");
        Serial.println(truncatedTemp, HEX);
    }

    return truncatedTemp;
}

void Send_LoRa_Data
(
    uint8_t mux_code, 
    float outside_temperature, float outside_CO2, float outside_humidity, 
    float battery_level, 
    float soil_nutrients_N_Nitrogen, float soil_nutrients_P_Phosphorus, float soil_nutrients_K_Potassium, 
    float surface_temperature, float surface_humidity, 
    float deep_temperature, float deep_humidity,
    float light_intensity
) {
    uint8_t NB_BITS = 20;
    uint8_t payload[NB_BITS] = {0};

    // 1️⃣ Add mux_code (1 byte)
    payload[0] = mux_code << 4;

    // 2️⃣ 1_HEX byte temperature compression
    //uint8_t compressed_outside_temperature = compress_2_HEX(outside_temperature * 5);
    //payload[0] += (compressed_outside_temperature >> 4);  // 4 high-order bits
    //payload[1] = ((compressed_outside_temperature << 4)); // 4 low-order bits

    // 3️⃣ 1.5_HEX byte CO2 compression
    uint16_t compressed_outside_CO2 = compress_3_HEX(outside_CO2);
    payload[0] += (compressed_outside_CO2) >> 8;  // 4 high-order bits
    payload[1] =  (compressed_outside_CO2);       // 8 low-order bits

    // 4️⃣ 1.5_HEX byte humidity compression
    uint16_t compressed_outside_humidity = compress_3_HEX(outside_humidity*25); // *10 *2,5 for +/-0.015 
    payload[2] = (compressed_outside_humidity) >> 4;  // 8 high-order bits
    payload[3] +=(compressed_outside_humidity) << 8;  // 4 low-order bits

    // 5️⃣ 1_HEX byte battery compression
    uint8_t compressed_battery_level = compress_2_HEX(battery_level * 10 / 4 );
    payload[3] += (compressed_battery_level) >> 4;  // 4 high-order bits
    payload[4] += (compressed_battery_level) << 4;  // 4 low-order bits

    // 6️⃣ 1.5_HEX byte N_nutriment compression
    uint16_t compressed_soil_nutrients_N_Nitrogen = compress_3_HEX(soil_nutrients_N_Nitrogen); 
    payload[4] += (compressed_soil_nutrients_N_Nitrogen) >> 8;  // 4 high-order bits
    payload[5] =  (compressed_soil_nutrients_N_Nitrogen);       // 8 low-order bits

    // 7️⃣ 1.5_HEX byte P_nutriment compression
    uint16_t compressed_soil_nutrients_P_Phosphorus = compress_3_HEX(soil_nutrients_P_Phosphorus); 
    payload[6] =  (compressed_soil_nutrients_P_Phosphorus) >> 4;  // 8 high-order bits
    payload[7] += (compressed_soil_nutrients_P_Phosphorus) << 4;  // 4 low-order bits

    // 8️⃣ 1.5_HEX byte K_nutriment compression
    uint16_t compressed_soil_nutrients_K_Potassium = compress_3_HEX(soil_nutrients_K_Potassium); 
    payload[7] += (compressed_soil_nutrients_K_Potassium) >> 8;  // 4 high-order bits
    payload[8] =  (compressed_soil_nutrients_K_Potassium);       // 8 low-order bits

    // 9️⃣ 1.5_HEX byte temperature compression
    uint16_t compressed_surface_temperature = compress_3_HEX(surface_temperature*25); // *10 *2,5 for +/-0.015 
    payload[9] += (compressed_surface_temperature) >> 4; // 8 high-order bits
    payload[10] += (compressed_surface_temperature) << 4; // 4 low-order bits

    // 1️⃣0️⃣ 1.5_HEX byte humidity compression
    uint16_t compressed_surface_humidity = compress_3_HEX(surface_humidity*25); // *10 *2,5 for +/-0.015 
    payload[10] += (compressed_surface_humidity) >> 8; // 4 high-order bits
    payload[11] =  (compressed_surface_humidity);      // 8 low-order bits

    // 1️⃣1️⃣ 1.5_HEX byte temperature compression
    uint16_t compressed_deep_temperature = compress_3_HEX(deep_temperature*25); // *10 *2,5 for +/-0.015 
    payload[12] += (compressed_deep_temperature) >> 4;  // 8 high-order bits
    payload[13] += (compressed_deep_temperature) << 4;  // 4 low-order bits

    // 1️⃣2️⃣ 1.5_HEX byte humidity compression
    uint16_t compressed_deep_humidity = compress_3_HEX(deep_humidity*25); // *10 *2,5 for +/-0.015 
    payload[13] += (compressed_deep_humidity) >> 8;  // 4 high-order bits
    payload[14] += (compressed_deep_humidity);       // 8 low-order bits

    // 1️⃣3️⃣ 1.5_HEX byte light compression
    uint16_t compressed_light_intensity = compress_3_HEX(light_intensity/10); 
    payload[15] += (compressed_light_intensity) >> 4;  // 4 high-order bits
    payload[16] =  (compressed_light_intensity) << 4;  // 8 low-order bits

    // 1️⃣4️⃣ 2 HEX SIGNED temperature compression
    int16_t compressed_outside_temperature = round(outside_temperature * 100);
    payload[17] = (compressed_outside_temperature >> 8);   // 8 high-order bits
    payload[18] = (compressed_outside_temperature);        // 8 low-order bits

    // 1️⃣6️⃣ Fill the rest of the payload with 0x00 (padding up to 51 bytes)
    for (int i = 19; i < NB_BITS; i++) payload[i] = 0x00;

    // 1️⃣7️⃣ Debug print payload 
    if (Test_Mode) 
    {
        Serial.print("\nSending LoRa -> Payload: ");
        for (int i = 0; i < NB_BITS; i++) 
        {
            if (payload[i] < 0x10) Serial.print("0");
            Serial.print(payload[i], HEX);
            Serial.print(" ");
        }
        Serial.println();
    }

    int attempt = 0, err = 0;
    do {
        if (Test_Mode) Serial.println("\nAttempting to send LoRa message...");
        MKR1013modem.beginPacket();
        MKR1013modem.write(payload, sizeof(payload));
        err = MKR1013modem.endPacket(true);

        if (err > 0) 
        {
            if (Test_Mode) Serial.println("✅ Message sent successfully!");
            break;
        } 
        else 
        {
            if (Test_Mode) Serial.println("❌ Sending error, check antenna!");
            delay(5000);
            attempt++;
        }
    } while (attempt < 3);

    if (attempt == 3 && err <= 0 && Test_Mode) 
    {
        Serial.println("❌ Max attempts reached. Message failed.");
    }
}

void LoraUnitShipment(float value) 
{
    uint8_t compressedTemp = compress_2_HEX(value);
    int err = 0;

    do 
    {
        if (Test_Mode) Serial.println("Attempting to send LoRa message...");
        MKR1013modem.beginPacket();
        MKR1013modem.write(compressedTemp);
        err = MKR1013modem.endPacket(true);

        if (err > 0 && Test_Mode) Serial.println("✅ Sending successful!");
        else if (Test_Mode) Serial.println("⚠️ Error, check Antenna! Retrying...");
        delay(5000);
    } while (err <= 0);
}
