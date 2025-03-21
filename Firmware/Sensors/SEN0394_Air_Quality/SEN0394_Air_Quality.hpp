#ifndef AIR_SENSOR_HPP
#define AIR_SENSOR_HPP

#include <DFRobot_SGP40.h>
#define TEST_MODE 0

class AirSensor
{
public:
    AirSensor();

    bool begin(uint8_t maxAttempts);
    float readAirQuality();

private:
    DFRobot_SGP40 mySgp40;
    bool initSensor(uint8_t maxAttempts);
};

#endif // AIR_SENSOR_HPP
