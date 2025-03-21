#ifndef _DFROBOT_SEN0308_H_
#define _DFROBOT_SEN0308_H_

#include <stdint.h>

#define TEST_MODE 1
#define AIR_HUMIDITY 890
#define WATER_HUMIDITY 1
class SoilMoistureSensor_SEN0308
{
    uint8_t _analog_pin;
    const float air_humidity;
    const float water_humidity;

    public:
    SoilMoistureSensor_SEN0308(const float air_humidity = AIR_HUMIDITY, const float water_humidity = WATER_HUMIDITY): air_humidity(air_humidity), water_humidity(water_humidity) {};
    bool begin(const uint8_t analog_pin);
    uint16_t read(bool percentage = true);
};
#endif //_DFROBOT_SEN0308_H_