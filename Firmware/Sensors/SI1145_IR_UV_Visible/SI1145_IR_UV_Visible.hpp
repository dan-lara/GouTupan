#ifndef SI1145_IR_UV_VISIBLE_HPP
#define SI1145_IR_UV_VISIBLE_HPP

#include <Wire.h>
#include "Arduino.h"

#define DEFAULT_I2C_ADDR 0x60
#define MAX_SENSOR_ATTEMPTS 5
#define TEST_MODE 1

class SI1145_IR_UV_Visible
{
public:
    SI1145_IR_UV_Visible();
    bool begin(uint8_t max_attempts = MAX_SENSOR_ATTEMPTS);
    uint16_t readVisible();
    uint16_t readIR();
    uint16_t readRegister(uint8_t reg);
    float readUV();

private:
    uint8_t _i2cAddr;
};

#endif // SI1145_IR_UV_VISIBLE_HPP