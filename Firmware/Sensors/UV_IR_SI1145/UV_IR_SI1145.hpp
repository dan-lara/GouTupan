#ifndef UV_IR_SI1145_HPP
#define UV_IR_SI1145_HPP

#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_SI1145.h"

bool setupUVIRSensor();
void getUVIRData(float &visible, float &infrared, float &uv);

#endif
