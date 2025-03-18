#include "SHT3xSensor.hpp"

<<<<<<< Updated upstream
/* 
 * _sht3x depends on _addr and _wire, 
 * but if they are placed later in the constructor initialization list, 
 * which may cause _sht3x to fail to use _addr and _wire correctly during initialization.
 */
SHT3xSensor::SHT3xSensor(uint8_t addr, TwoWire* wire) 
    : _addr(addr), _wire(wire), _sht3x(addr, wire) {}

/*
 * Wire.begin() can only be called once, usually in setup().
 * If the main program (such as setup()) has already called Wire.begin(),  
 * "the _wire->begin();" may cause I2C to be reinitialized, affecting communication.
 */
bool SHT3xSensor::begin() 
{
    if (!_wire) return false;
    if (!_sht3x.begin()) return false;
    return true;
}

std::pair<float, float> SHT3xSensor::measure() {
    if (_sht3x.measure()) {
        return {_sht3x.temperature(), _sht3x.humidity()};
    }
    return {0.0, 0.0};
=======
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
>>>>>>> Stashed changes
}