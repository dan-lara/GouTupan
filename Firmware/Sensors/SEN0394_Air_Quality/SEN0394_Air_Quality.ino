#include <Wire.h>
#include "SEN0394_Air_Quality.hpp"
#define NB_MAX_SENSOR_ATTEMPT 5

AirSensor airSensor;

void setup() 
{   
    airSensor.begin(NB_MAX_SENSOR_ATTEMPT);
}

void loop() 
{
    float quality = airSensor.readAirQuality();

    #if TEST_MODE
        Serial.print("Air UnQuality Index : ");
        Serial.println(quality);
    #endif

    delay(1000);
}
