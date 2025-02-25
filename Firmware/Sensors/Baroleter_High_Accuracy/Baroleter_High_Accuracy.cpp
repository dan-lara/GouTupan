#include "Baroleter_High_Accuracy.hpp"

#define CMD_SOFT_RESET 0x06
#define CMD_READ_P 0x30
#define CMD_CONVERT 0x40

// Constructor
HP20x::HP20x(uint8_t address) : _address(address) 
{
}

void HP20x::begin() 
{
    Wire.begin();

    // Check sensor connection
    Wire.beginTransmission(_address);
    if (Wire.endTransmission() != 0) 
    {
        while (1);  // Stop execution if the sensor is not detected
    }

    // Soft reset the sensor
    sendCommand(CMD_SOFT_RESET);
    delay(100);
}

float HP20x::readPressure() 
{
    sendCommand(CMD_CONVERT);
    delay(25);
    
    sendCommand(CMD_READ_P);
    long pressure = readData();

    return pressure / 100.0;  // Convert raw data to hPa
}

void HP20x::sendCommand(uint8_t cmd) 
{
    Wire.beginTransmission(_address);
    Wire.write(cmd);
    Wire.endTransmission();
}

long HP20x::readData() 
{
    Wire.requestFrom(_address, (uint8_t)3);
    if (Wire.available() < 3) 
    {
        return -1;  // Return error code if data is not available
    }

    long data = 0;
    data |= Wire.read() << 16;
    data |= Wire.read() << 8;
    data |= Wire.read();
    return data;
}
