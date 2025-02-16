#ifndef LORA_TRANSMISSION_HPP
#define LORA_TRANSMISSION_HPP

#include <MKRWAN.h>

// Déclaration des fonctions
void initializeLoRa();
void sendLoRaMessage(float value);
uint8_t compressTemperature(float temperature);

#endif
