#ifndef COLORSENSOR_H
#define COLORSENSOR_H

#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"

// Function declarations
bool initColorSensor();
void readAndNormalizeColor(float* r, float* g, float* b, float* light);

#endif
