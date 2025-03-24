#ifndef NPK_HPP
#define NPK_HPP

#include <Arduino.h>

class SoilSensor {
public:
    SoilSensor(HardwareSerial& serial);

    float readN();
    float readP();
    float readK();

private:
    HardwareSerial& serialPort;

    uint8_t Com[8]  = { 0x01, 0x03, 0x00, 0x1E, 0x00, 0x01, 0xE4, 0x0C }; // 氮
    uint8_t Com1[8] = { 0x01, 0x03, 0x00, 0x1F, 0x00, 0x01, 0xB5, 0xCC }; // 磷
    uint8_t Com2[8] = { 0x01, 0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xC0 }; // 钾

    uint8_t readNByte(uint8_t* buf, size_t len);
    unsigned int CRC16_2(uint8_t* buf, int len);
};

#endif
