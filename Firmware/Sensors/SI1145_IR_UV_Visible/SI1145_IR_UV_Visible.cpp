#include "SI1145_IR_UV_Visible.hpp"

SI1145_IR_UV_Visible::SI1145_IR_UV_Visible()
{
    _i2cAddr = DEFAULT_I2C_ADDR;
}

bool SI1145_IR_UV_Visible::begin(uint8_t max_attempts)
{
    Wire.begin();
    uint8_t attempts = 0;
    while (attempts < max_attempts)
    {
        if (readRegister(0x00) != 0x00)
        {
            return true;
        }
        attempts++;
        delay(1000);
    }
    return false;
}

uint16_t SI1145_IR_UV_Visible::readVisible()
{
    return readRegister(0x22);
}

uint16_t SI1145_IR_UV_Visible::readIR()
{
    Wire.beginTransmission(_i2cAddr);
    Wire.write(0x24);
    Wire.endTransmission();
    delay(10);
    Wire.requestFrom(_i2cAddr, (uint8_t)2);
    if (Wire.available() >= 2)
    {
        uint16_t value = Wire.read();
        value |= (Wire.read() << 8);
        return value;
    }
    return 0;
}

float SI1145_IR_UV_Visible::readUV()
{
    readRegister(0x2C);
    delay(10);
    return readRegister(0x2C);
}

uint16_t SI1145_IR_UV_Visible::readRegister(uint8_t reg)
{
    Wire.beginTransmission(_i2cAddr);
    Wire.write(reg);
    Wire.endTransmission();
    delay(40);
    Wire.requestFrom(_i2cAddr, (uint8_t)2);
    if (Wire.available() >= 2)
    {
        uint16_t value = Wire.read();
        value |= (Wire.read() << 8);
        return value;
    }
    return 0;
}