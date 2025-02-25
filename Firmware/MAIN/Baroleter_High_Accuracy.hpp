#ifndef BAROLETER_HIGH_ACCURACY_HPP
#define BAROLETER_HIGH_ACCURACY_HPP

#include <Wire.h>
#include <Arduino.h>  // Needed for delay() and Serial

class HP20x
{
public:
    HP20x(uint8_t address = 0x76);
    void begin();
    float readPressure();  // Returns pressure in hPa

private:
    uint8_t _address;
    void sendCommand(uint8_t cmd);
    long readData();
};

#endif // BAROLETER_HIGH_ACCURACY_HPP
