// GasSensor.hpp
#ifndef GAS_SENSOR_HPP
#define GAS_SENSOR_HPP

#include <Wire.h>
#include <Arduino.h>

#define I2C_ADDR 0x04

#define CMD_READ_NH3    1
#define CMD_READ_CO     2
#define CMD_READ_NO2    3
#define CMD_READ_C3H8   4
#define CMD_READ_C4H10  5
#define CMD_READ_CH4    6
#define CMD_READ_H2     7
#define CMD_READ_C2H5OH 8
#define CMD_VERSION     9
#define CMD_POWER_ON    10
#define CMD_POWER_OFF   11

#define TEST_MODE 1

void setupGasSensor();
void readGasValues(float &nh3, float &co, float &no2, float &c3h8, float &c4h10, float &ch4, float &h2, float &c2h5oh);
int getVersion();
void powerOn();
float readGasConcentration(uint8_t command);
int16_t readData(uint8_t command);
float convertADCtoPPM(int adcValue);
#endif
