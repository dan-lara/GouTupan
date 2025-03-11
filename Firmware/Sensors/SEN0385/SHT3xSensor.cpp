#include "SHT3xSensor.hpp"

SHT3xSensor::SHT3xSensor(uint8_t addr, TwoWire* wire) 
    : _sht3x(addr, wire), _addr(addr), _wire(wire) {}

bool SHT3xSensor::begin() 
{
    _wire->begin();
    return _sht3x.begin();
}

bool SHT3xSensor::measure() 
{
    return _sht3x.measure();
}

float SHT3xSensor::temperature() 
{  
    return _sht3x.temperature();
}

float SHT3xSensor::humidity() 
{     
    return _sht3x.humidity();
}