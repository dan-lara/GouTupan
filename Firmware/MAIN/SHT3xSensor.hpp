#ifndef SHT3XSENSOR_HPP
#define SHT3XSENSOR_HPP

#include <Arduino.h>
#include <Wire.h>
#include <ArtronShop_SHT3x.h>
<<<<<<< Updated upstream
#include <utility>
=======
>>>>>>> Stashed changes

class SHT3xSensor 
{
public:
    SHT3xSensor(uint8_t addr = 0x44, TwoWire* wire = &Wire);
    bool begin();
<<<<<<< Updated upstream
    std::pair<float, float> measure(); 
=======
    bool measure();
    float temperature();  
    float humidity();     
>>>>>>> Stashed changes

private:
    ArtronShop_SHT3x _sht3x;
    uint8_t _addr;
    TwoWire* _wire;
};

#endif // SHT3XSENSOR_HPP