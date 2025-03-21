#include "Baroleter_High_Accuracy.hpp"

#define BAROMETER_ADDRESS 0x76

void sendCommand(uint8_t cmd) 
{
    Wire.beginTransmission(BAROMETER_ADDRESS);
    Wire.write(cmd);
    Wire.endTransmission();
}

void setupBarometer() 
{
    Wire.begin();
    
    Wire.beginTransmission(BAROMETER_ADDRESS);
    if (Wire.endTransmission() != 0) 
    {
        while (1);  // Stop si le capteur est absent
    }

    sendCommand(0x06);  // Reset du capteur
    delay(100);
}

float getPressure() 
{
    sendCommand(0x40);  // Conversion
    delay(25);

    sendCommand(0x30);  // Lire la pression
    delay(10);
    
    Wire.requestFrom(BAROMETER_ADDRESS, (uint8_t)3);
    if (Wire.available() < 3) 
    {
        return -1;  // Erreur
    }

    long pressure = 0;
    pressure |= Wire.read() << 16;
    pressure |= Wire.read() << 8;
    pressure |= Wire.read();

    return pressure / 100.0;
}
