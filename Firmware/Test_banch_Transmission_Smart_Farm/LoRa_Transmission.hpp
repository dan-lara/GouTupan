#ifndef LORA_TRANSMISSION_HPP
#define LORA_TRANSMISSION_HPP

#include <MKRWAN.h>

// Déclaration des fonctions
void initializeLoRa();
void LoraUnitShipment(float value);
void Send_LoRa_Data(
    uint8_t mux_code, 
    float outside_temperature, float outside_CO2, float outside_humidity, 
    float battery_level, 
    float soil_nutrients_N_Nitrogen, float soil_nutrients_P_Phosphorus, float soil_nutrients_K_Potassium, 
    float surface_temperature, float surface_humidity, 
    float deep_temperature, float deep_humidity,
    float light_intensity
);
uint8_t compress_2_HEX (float value); 
uint16_t compress_3_HEX(float no_compressed);

#endif
