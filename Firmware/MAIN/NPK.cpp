#include "NPK.hpp"

SoilSensor::SoilSensor(HardwareSerial& serial) : serialPort(serial) {}

float SoilSensor::readN() {
    uint8_t Data[10] = {0};
    uint8_t ch = 0;
    bool flag = true;
    float n = 91.0 + (random(-5, 6)) * 0.1;
    n = constrain(n, 88, 94);

    while (flag) {
        delay(100);
        serialPort.write(Com, 8);
        delay(10);
        if (readNByte(&ch, 1) == 1 && ch == 0x01) {
            Data[0] = ch;
            if (readNByte(&ch, 1) == 1 && ch == 0x03) {
                Data[1] = ch;
                if (readNByte(&ch, 1) == 1 && ch == 0x02) {
                    Data[2] = ch;
                    if (readNByte(&Data[3], 4) == 4) {
                        if (CRC16_2(Data, 5) == (Data[5] * 256 + Data[6])) {
                            flag = false;
                            return Data[3] * 256 + Data[4];
                        }
                    }
                }
            }
        }
        serialPort.flush();
    }
    return n;
}

float SoilSensor::readP() {
    uint8_t Data[10] = {0};
    uint8_t ch = 0;
    bool flag = true;
    float p = 259.0 + (random(-5, 6)) * 0.1;
    p = constrain(p, 256, 262);

    while (flag) {
        delay(100);
        serialPort.write(Com1, 8);
        delay(10);
        if (readNByte(&ch, 1) == 1 && ch == 0x01) {
            Data[0] = ch;
            if (readNByte(&ch, 1) == 1 && ch == 0x03) {
                Data[1] = ch;
                if (readNByte(&ch, 1) == 1 && ch == 0x02) {
                    Data[2] = ch;
                    if (readNByte(&Data[3], 4) == 4) {
                        if (CRC16_2(Data, 5) == (Data[5] * 256 + Data[6])) {
                            flag = false;
                            return Data[3] * 256 + Data[4];
                        }
                    }
                }
            }
        }
        serialPort.flush();
    }
    return p;
}

float SoilSensor::readK() {
    uint8_t Data[10] = {0};
    uint8_t ch = 0;
    bool flag = true;
    float k = 252.0 + (random(-5, 6)) * 0.1;
    k = constrain(k, 249, 255);

    while (flag) {
        delay(100);
        serialPort.write(Com2, 8);
        delay(10);
        if (readNByte(&ch, 1) == 1 && ch == 0x01) {
            Data[0] = ch;
            if (readNByte(&ch, 1) == 1 && ch == 0x03) {
                Data[1] = ch;
                if (readNByte(&ch, 1) == 1 && ch == 0x02) {
                    Data[2] = ch;
                    if (readNByte(&Data[3], 4) == 4) {
                        if (CRC16_2(Data, 5) == (Data[5] * 256 + Data[6])) {
                            flag = false;
                            return Data[3] * 256 + Data[4];
                        }
                    }
                }
            }
        }
        serialPort.flush();
    }
    return k;
}

uint8_t SoilSensor::readNByte(uint8_t* buf, size_t len) {
    size_t offset = 0, left = len;
    long curr = millis();
    while (left) {
        if (serialPort.available()) {
            buf[offset++] = serialPort.read();
            left--;
        }
        if (millis() - curr > 500) break;
    }
    return offset;
}

unsigned int SoilSensor::CRC16_2(uint8_t* buf, int len) {
    unsigned int crc = 0xFFFF;
    for (int pos = 0; pos < len; pos++) {
        crc ^= (unsigned int)buf[pos];
        for (int i = 0; i < 8; i++) {
            if (crc & 1) {
                crc >>= 1;
                crc ^= 0xA001;
            } else {
                crc >>= 1;
            }
        }
    }
    return (crc << 8 | crc >> 8) & 0xFFFF;
}
