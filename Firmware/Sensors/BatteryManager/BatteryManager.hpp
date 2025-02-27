#ifndef BATTERY_MANAGER_H
#define BATTERY_MANAGER_H

#define TEST_MODE 0

#include <cstdint>

#define MAX_VOLTAGE 3.223
#define MIN_VOLTAGE 2.763

class BatteryManager
{
private:
    bool charging;
    bool fault;
    float voltage;
    uint8_t _analog_pin;
    uint8_t _charge_pin;
    uint8_t _fault_pin;
public:
    bool begin(uint8_t analog_pin, uint8_t charge_pin, uint8_t fault_pin);
    float read(bool percentage = true);
    bool isCharging();
    bool isFault();
};
#endif
