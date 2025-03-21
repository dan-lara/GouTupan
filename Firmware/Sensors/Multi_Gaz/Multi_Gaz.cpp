// GasSensor.cpp
#include "Multi_Gaz.hpp"

void setupGasSensor() 
{
    Wire.begin();
    
    #if TEST_MODE
    Serial.begin(115200);
    Serial.println("Power on!");
    #endif
    
    powerOn();
    int version = getVersion();
    
    #if TEST_MODE
    Serial.print("Firmware Version = ");
    Serial.println(version);
    #endif
}

void readGasValues(float &nh3, float &co, float &no2, float &c3h8, float &c4h10, float &ch4, float &h2, float &c2h5oh) {
    nh3 = readGasConcentration(CMD_READ_NH3);
    co = readGasConcentration(CMD_READ_CO);
    no2 = readGasConcentration(CMD_READ_NO2);
    c3h8 = readGasConcentration(CMD_READ_C3H8);
    c4h10 = readGasConcentration(CMD_READ_C4H10);
    ch4 = readGasConcentration(CMD_READ_CH4);
    h2 = readGasConcentration(CMD_READ_H2);
    c2h5oh = readGasConcentration(CMD_READ_C2H5OH);
    
    #if TEST_MODE
    Serial.print("NH3: "); Serial.println(nh3);
    Serial.print("CO: "); Serial.println(co);
    Serial.print("NO2: "); Serial.println(no2);
    Serial.print("C3H8: "); Serial.println(c3h8);
    Serial.print("C4H10: "); Serial.println(c4h10);
    Serial.print("CH4: "); Serial.println(ch4);
    Serial.print("H2: "); Serial.println(h2);
    Serial.print("C2H5OH: "); Serial.println(c2h5oh);
    Serial.println("...");
    #endif
}

int getVersion() {
    return readData(CMD_VERSION);
}

void powerOn() {
    Wire.beginTransmission(I2C_ADDR);
    Wire.write(CMD_POWER_ON);
    Wire.endTransmission();
}

float readGasConcentration(uint8_t command) {
    int16_t rawValue = readData(command);
    if (rawValue < 0) return 0.1;
    return convertADCtoPPM(rawValue);
}

int16_t readData(uint8_t command) {
    Wire.beginTransmission(I2C_ADDR);
    Wire.write(command);
    Wire.endTransmission();
    
    delay(2);
    Wire.requestFrom(I2C_ADDR, 2);
    
    if (Wire.available() != 2) 
    {
        return -1;
    }
    
    int16_t value = Wire.read() << 8 | Wire.read();
    return value;
}

float convertADCtoPPM(int adcValue) {
    return (float)adcValue / 1024.0 * 100;
}
