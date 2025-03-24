#define NB_MAX_SENSOR_ATTEMPT 5

#include "SEN0332_O2.hpp"

DFRobot_OxygenSensor oxygen;

void setup(void)
{
  #if TEST_MODE
      Serial.begin(115200);
  #endif
  oxygen.setup_O2_sensor(NB_MAX_SENSOR_ATTEMPT);
}

void loop(void)
{
  float O2 = oxygen.getOxygenData(10);
  #if TEST_MODE
      Serial.print(" Oxygen concentration is ");
      Serial.print(O2);
      Serial.println(" ppm");
  #endif
  delay(1000);
}

