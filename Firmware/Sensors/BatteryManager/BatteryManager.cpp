#include "BatteryManager.hpp"
#include <Arduino.h>

bool BatteryManager::begin(uint8_t analog_pin, uint8_t charge_pin, uint8_t fault_pin)
{
    _analog_pin = analog_pin;
    _charge_pin = charge_pin;
    _fault_pin = fault_pin;

    pinMode(_charge_pin, INPUT);
    pinMode(_fault_pin, INPUT);
    pinMode(_analog_pin, INPUT);

    return read() > 0;
}

float BatteryManager::read(bool percentage)
{
    const uint16_t samples = 5; // Fixed number of samples
    const uint16_t delay_ms = 1; // Fixed delay in milliseconds between each sample reading
    uint64_t accumulator = 0;

    for (uint16_t i = 0; i < samples; i++) {
        delay(delay_ms);
        accumulator += analogRead(_analog_pin);
    }

    voltage = float(accumulator) / samples;

    voltage = float(voltage) * (MAX_VOLTAGE - MIN_VOLTAGE) / 1023.0 + MIN_VOLTAGE;

    #if TEST_MODE
        Serial.print("Battery Voltage: ");
        Serial.println(voltage);
    #endif

    if (percentage)
    {
        return (voltage - MIN_VOLTAGE) * 100.0 / (MAX_VOLTAGE - MIN_VOLTAGE);
    }
    return voltage;
}

bool BatteryManager::isCharging()
{
    charging = digitalRead(_charge_pin);

    #if TEST_MODE
        Serial.print("Charging: ");
        Serial.println(charging);
    #endif

    return charging;
}

bool BatteryManager::isFault()
{
    fault = digitalRead(_fault_pin);

    #if TEST_MODE
        Serial.print("Fault: ");
        Serial.println(fault);
    #endif

    return fault;
}