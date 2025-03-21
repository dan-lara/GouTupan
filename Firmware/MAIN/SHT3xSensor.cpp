#include "SHT3xSensor.hpp"

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
}
