#ifndef LORA_TRANSMISSION_HPP
#define LORA_TRANSMISSION_HPP

#include <MKRWAN.h>

// Déclaration des fonctions
void initializeLoRa();
void LoraUnitShipment(float value);
void Send_LoRa_Data(uint8_t mux_code, float outside_temperature, float outside_CO2, float outside_humidity);
uint8_t compressTemperature(float temperature);
uint16_t compress_3_HEX(float no_compressed);

#endif
