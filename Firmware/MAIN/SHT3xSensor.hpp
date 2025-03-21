#ifndef SHT3XSENSOR_HPP
#define SHT3XSENSOR_HPP

#include <Arduino.h>
#include <Wire.h>
#include <ArtronShop_SHT3x.h>
#include <utility>

class SHT3xSensor 
{
public:
    SHT3xSensor(uint8_t addr = 0x44, TwoWire* wire = &Wire);
    bool begin();
    std::pair<float, float> measure(); 

private:
    ArtronShop_SHT3x _sht3x;
    uint8_t _addr;
    TwoWire* _wire;
};

#endif // SHT3XSENSOR_HPP
