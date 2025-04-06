#include "LoRa_Transmission.hpp"
#include "Arduino_Key.hpp"  // Contient les constantes APPEUI et APPKEY

LoRaModem MKR1013modem;

FlashStorage(storage, Late_Payload_Flash_Data);

void initializeLoRa() 
{
    #if TEST_MODE
        Serial.begin(115200);
        while (!Serial);
        Serial.println("\nStarting LoRa module...");
    #endif
  
    if (!MKR1013modem.begin(EU868)) 
    {
        #if TEST_MODE
            Serial.println("LoRa module startup failed!");
        #endif
        while (1);
    }

    #if TEST_MODE
        Serial.print("Firmware version: ");
        Serial.println(MKR1013modem.version());

        Serial.print("Device EUI: ");
        Serial.println(MKR1013modem.deviceEUI());

        Serial.println("Connecting to TTN...");
    #endif

    int connected = MKR1013modem.joinOTAA(APPEUI, APPKEY);

    if (!connected) 
    {
        #if TEST_MODE
            Serial.println("Connection failed! Check your signal and retry.");
        #endif
        while (1);
    }

    #if TEST_MODE
        Serial.println("Connected to TTN!");
        Serial.println("\n⚙️ Data Compression Precision :");
        Serial.println("Outside Temperature ±0.01/2°C  |  Outside CO2 ±0.5 ppm  |  Outside Humidity ±0.025/2 %");
        Serial.println("Battery Level ±0.2%");
        Serial.println("NPK ±0.5 ppm  |  Surface Humidity ±0.025/2 %  |  Surface Temperature ±0.025/2 °C ");
        Serial.println("Deep Temperature ±0.025/2 °C  |  Deep Humidity ±0.025/2 %");
        Serial.println("Lux Sensor ±5 lux ( Visible + IR + UV )");
        Serial.println("RGB Sensor ±1/2 HEX_Code ( RR + GG + BB )");
        Serial.println("Pressure ±0.15 bar");
        Serial.println();  // Empty line for better readability
    #endif

    MKR1013modem.minPollInterval(10);
}

uint8_t compress_2_HEX(float value) 
{
    float truncatedValue = round(value);
    uint16_t intValue = (uint16_t)truncatedValue;
    uint8_t compressedValue = (uint8_t)min(intValue, 405);

    #if TEST_MODE
        Serial.print("Original value: ");
        Serial.print(value);
        Serial.print(" -> Rounded: ");
        Serial.print(intValue);
        Serial.print(" -> Compressed: 0x");
        if (compressedValue < 0x10) Serial.print("0");
        Serial.println(compressedValue, HEX);
    #endif

    return compressedValue;
}

uint16_t compress_3_HEX(float no_compressed) 
{
    uint16_t truncatedTemp = round(no_compressed);

    #if TEST_MODE
        Serial.print("Original data: ");
        Serial.print(no_compressed);
        Serial.print(" -> Compressed: 0x");
        if (truncatedTemp < 0x10) Serial.print("0");
        if (truncatedTemp < 0x100) Serial.print("0");
        Serial.println(truncatedTemp, HEX);
    #endif

    return truncatedTemp;
}

bool sendPayload(uint8_t *payload) 
{
    #if TEST_MODE
        Serial.println("TEST MODE: Payload transmission trying to send back.");
    #endif

    MKR1013modem.beginPacket();
    MKR1013modem.write(payload, NB_BITS_PAYLOAD);
    int err = MKR1013modem.endPacket(true);

    if (err == 1) 
    {
        return true;
    } 
    else 
    {
        //Serial.println("Error sending payload. Code: " + String(err));
        return false;
    }
}

void storePayload(uint8_t *payload) 
{
    Late_Payload_Flash_Data data;
    memcpy(data.payload, payload, NB_BITS_PAYLOAD);
    storage.write(data);
    #if TEST_MODE
        Serial.println("Payload stored in Flash.");
    #endif
}

void clearFlash() 
{
    Late_Payload_Flash_Data emptyData = {0}; // Fills the array with zeros
    storage.write(emptyData);
    #if TEST_MODE
        Serial.println("Flash memory cleared.");
    #endif
}

bool retrySendingStoredPayload() 
{
    #if TEST_MODE
        Serial.println("Checking Flash memory for unsent data...");
    #endif
    // Read stored payload from Flash
    Late_Payload_Flash_Data data = storage.read();

    // Check if the Flash memory contains valid data
    if (data.payload[0] == 0) 
    {
        #if TEST_MODE
            Serial.println("No pending data in Flash.");
        #endif
        return true;  // Nothing to send, return success
    }
    #if TEST_MODE
    Serial.println("Unsent data found. Retrying transmission...");
    #endif

    // Attempt to resend data up to MAX_ATTEMPTS times
    for (int attempt = 1; attempt <= MAX_ATTEMPTS; attempt++) 
    {
        #if TEST_MODE
            Serial.print("Attempt ");
            Serial.print(attempt);
            Serial.println("...");
            Serial.println("TEST MODE: Simulating payload transmission.");
        #endif
        if (sendPayload(data.payload)) 
        {
            clearFlash();  // Clear Flash after successful transmission
            return true;  // Successfully sent
        }
        delay(5000);  // Wait before retrying 5 sec
    }
    return false;  // Transmission failed
}


void Send_LoRa_Data
(
    uint8_t mux_code, 
    float outside_temperature, float outside_CO2, float outside_humidity, 
    float battery_level, 
    float soil_nutrients_N_Nitrogen, float soil_nutrients_P_Phosphorus, float soil_nutrients_K_Potassium, 
    float surface_temperature, float surface_humidity, 
    float deep_temperature, float deep_humidity,

    float light_intensity, float light_infrared, float light_ultraviolet,
    float R_RGB, float G_RGB, float B_RGB,
  
    float pressure, float quality, float O2,
    float nh3, float co, float no2, float c3h8, float c4h10, float ch4, float h2, float c2h5oh
) 
{
    uint8_t payload[NB_BITS_PAYLOAD] = {0};

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
    uint16_t compressed_outside_humidity = compress_3_HEX(outside_humidity*40); // *10 *2,5 for +/- 1/40 = 0.025 
    payload[2] = (compressed_outside_humidity) >> 4;  // 8 high-order bits
    payload[3] +=(compressed_outside_humidity) << 4;  // 4 low-order bits

    // 5️⃣ 1_HEX byte battery compression
    uint8_t compressed_battery_level = compress_2_HEX(battery_level * 10 / 4 );
    payload[3] += (compressed_battery_level) >> 4;  // 4 high-order bits
    payload[4] += (compressed_battery_level) << 4;  // 4 low-order bits

    // 6️⃣ 1.5_HEX byte N_nutriment compression
    uint16_t compressed_soil_nutrients_N_Nitrogen = compress_3_HEX(soil_nutrients_N_Nitrogen*10); 
    payload[4] += (compressed_soil_nutrients_N_Nitrogen) >> 8;  // 4 high-order bits
    payload[5] =  (compressed_soil_nutrients_N_Nitrogen);       // 8 low-order bits

    // 7️⃣ 1.5_HEX byte P_nutriment compression
    uint16_t compressed_soil_nutrients_P_Phosphorus = compress_3_HEX(soil_nutrients_P_Phosphorus*10); 
    payload[6] =  (compressed_soil_nutrients_P_Phosphorus) >> 4;  // 8 high-order bits
    payload[7] += (compressed_soil_nutrients_P_Phosphorus) << 4;  // 4 low-order bits

    // 8️⃣ 1.5_HEX byte K_nutriment compression
    uint16_t compressed_soil_nutrients_K_Potassium = compress_3_HEX(soil_nutrients_K_Potassium*10); 
    payload[7] += (compressed_soil_nutrients_K_Potassium) >> 8;  // 4 high-order bits
    payload[8] =  (compressed_soil_nutrients_K_Potassium);       // 8 low-order bits

    // 9️⃣ 1.5_HEX byte temperature compression
    uint16_t compressed_surface_temperature = compress_3_HEX(surface_temperature*40); // *10 *2,5 for +/- 1/40 = 0.025 
    payload[9] += (compressed_surface_temperature) >> 4; // 8 high-order bits
    payload[10] += (compressed_surface_temperature) << 4; // 4 low-order bits

    // 1️⃣0️⃣ 1.5_HEX byte humidity compression
    uint16_t compressed_surface_humidity = compress_3_HEX(surface_humidity*40); // *10 *2,5 for +/- 1/40 = 0.025 
    payload[10] += (compressed_surface_humidity) >> 8; // 4 high-order bits
    payload[11] =  (compressed_surface_humidity);      // 8 low-order bits

    // 1️⃣1️⃣ 1.5_HEX byte temperature compression
    uint16_t compressed_deep_temperature = compress_3_HEX(deep_temperature*40); // *10 *2,5 for +/- 1/40 = 0.025 
    payload[12] += (compressed_deep_temperature) >> 4;  // 8 high-order bits
    payload[13] += (compressed_deep_temperature) << 4;  // 4 low-order bits

    // 1️⃣2️⃣ 1.5_HEX byte humidity compression
    uint16_t compressed_deep_humidity = compress_3_HEX(deep_humidity*40); // *10 *4 for +/- 1/40 = 0.025 
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

    // 1️⃣3️⃣ 1.5_HEX byte light compression
    uint16_t compressed_light_infrared = compress_3_HEX(light_infrared/10); 
    payload[19] =  (compressed_light_infrared) >> 4;  // 8 high-order bits
    payload[20] += (compressed_light_infrared) << 4;  // 4 low-order bits

    // 1️⃣4️⃣ 1.5_HEX byte light compression
    uint16_t compressed_light_ultraviolet = compress_3_HEX(light_ultraviolet*100); 
    payload[20] += (compressed_light_ultraviolet) >> 8;  // 4 high-order bits
    payload[21] =  (compressed_light_ultraviolet);       // 8 low-order bits

    uint8_t compressed_R_RGB = compress_2_HEX(R_RGB);
    payload[22] = (compressed_R_RGB);  // 8 high-order bits

    uint8_t compressed_G_RGB = compress_2_HEX(G_RGB);
    payload[23] = (compressed_G_RGB);  // 8 high-order bits

    uint8_t compressed_B_RGB = compress_2_HEX(B_RGB);
    payload[24] = (compressed_B_RGB);  // 8 high-order bits

    uint16_t compressed_pressure = compress_3_HEX(pressure*3); 
    payload[25] =  (compressed_pressure) >> 4;  // 8 high-order bits
    payload[26] += (compressed_pressure) << 4;  // 4 low-order bits

    // 1️⃣5️⃣ 1.5_HEX byte GAZ compression
    uint16_t compressed_nh3 = compress_3_HEX(nh3*10); 
    payload[26] += (compressed_nh3) >> 8;  // 4 high-order bits
    payload[27] =  (compressed_nh3);       // 8 low-order bits

    uint16_t compressed_co = compress_3_HEX(co*10); 
    payload[28] =  (compressed_co) >> 4;  // 8 high-order bits
    payload[29] += (compressed_co) << 4;  // 4 low-order bits

    uint16_t compressed_no2 = compress_3_HEX(no2*10); 
    payload[29] += (compressed_no2) >> 8;  // 4 high-order bits
    payload[30] =  (compressed_no2);       // 8 low-order bits

    uint16_t compressed_c3h8 = compress_3_HEX(c3h8*10); 
    payload[31] =  (compressed_c3h8) >> 4;  // 8 high-order bits
    payload[32] += (compressed_c3h8) << 4;  // 4 low-order bits

    uint16_t compressed_c4h10 = compress_3_HEX(c4h10*10); 
    payload[32] += (compressed_c4h10) >> 8;  // 4 high-order bits
    payload[33] =  (compressed_c4h10);       // 8 low-order bits

    uint16_t compressed_ch4 = compress_3_HEX(ch4*10); 
    payload[34] =  (compressed_ch4) >> 4;  // 8 high-order bits
    payload[35] += (compressed_ch4) << 4;  // 4 low-order bits

    uint16_t compressed_h2 = compress_3_HEX(h2*10); 
    payload[35] += (compressed_h2) >> 8;  // 4 high-order bits
    payload[36] =  (compressed_h2);       // 8 low-order bits

    uint16_t compressed_c2h5oh = compress_3_HEX(c2h5oh*10); 
    payload[37] =  (compressed_c2h5oh) >> 4;  // 8 high-order bits
    payload[38] += (compressed_c2h5oh) << 4;  // 4 low-order bits

    // 1️⃣6️⃣ 1.5_HEX byte GAZ_Global_Polution compression
    uint16_t compressed_quality = compress_3_HEX(quality*5); 
    payload[38] += (compressed_quality) >> 8;  // 4 high-order bits
    payload[39] =  (compressed_quality);       // 8 low-order bits

    // 1️⃣7️⃣ 2 HEX byte O2 compression
    int16_t compressed_outside_O2 = round(O2 / 6);
    payload[40] = (compressed_outside_O2 >> 8);   // 8 high-order bits
    payload[41] = (compressed_outside_O2);        // 8 low-order bits

    // 1️⃣8️⃣ Fill the rest of the payload with 0x00 (padding up to 51 bytes)
    for (int i = 42; i < NB_BITS_PAYLOAD; i++) payload[i] = 0x00;

    // 1️⃣9️⃣ Debug print payload 
  
    #if TEST_MODE
        Serial.print("\nSending LoRa -> Payload: ");
        for (int i = 0; i < NB_BITS_PAYLOAD; i++) 
        {
            if (payload[i] < 0x10) Serial.print("0");
            Serial.print(payload[i], HEX);
            Serial.print(" ");
        }
        Serial.println();
    #endif

    int attempt = 0, err = 0;
    do {
        #if TEST_MODE
            Serial.println("\nAttempting to send LoRa message...");
        #endif
        MKR1013modem.beginPacket();
        MKR1013modem.write(payload, sizeof(payload));
        err = MKR1013modem.endPacket(true);

        if (err > 0) 
        {
            #if TEST_MODE
                Serial.println("✅ Message sent successfully!");
            #endif
            break;
        } 
        else 
        {
            #if TEST_MODE
                Serial.println("❌ Sending error, check antenna!");
            #endif
            delay(1000); // 1 sec
            attempt++;
        }
    } while (attempt < MAX_ATTEMPTS);

    #if TEST_MODE
        if (attempt == MAX_ATTEMPTS && err <= 0) 
        {
            Serial.println("❌ Max attempts reached. Message failed.");
        }
    #endif
    storePayload(payload);
}

void LoraUnitShipment(float value) 
{
    uint8_t compressedTemp = compress_2_HEX(value);
    int err = 0;

    do 
    {
        #if TEST_MODE
            Serial.println("Attempting to send LoRa message...");
        #endif
        MKR1013modem.beginPacket();
        MKR1013modem.write(compressedTemp);
        err = MKR1013modem.endPacket(true);

        #if TEST_MODE
            if (err > 0) Serial.println("✅ Sending successful!");
            else Serial.println("⚠️ Error, check Antenna! Retrying...");
        #endif

        delay(1000);
    } while (err <= 0);
}