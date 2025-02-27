#ifndef BATTERY_MANAGER_H
#define BATTERY_MANAGER_H

#define TEST_MODE 0

#include <cstdint>

//Resistor divider values :
// R1 = 100k
// R2 = 330k

#define MAX_VOLTAGE 3.223 // 4.2 is the maximum voltage for a LiPo battery
// #define MIN_VOLTAGE 2.763 // 3.6 is the minimum voltage for a LiPo battery
#define MIN_VOLTAGE 2.533 // 3.3 is the minimum voltage for a LiPo battery


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
    float getVoltage();
    float getPercentage();
    bool isCharging();
    bool isFault();
};
#endif
