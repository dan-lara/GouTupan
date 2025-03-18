#include "ColorSensor.hpp"

// Create a color sensor object
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

bool initColorSensor() {
    return tcs.begin();
}

void readAndNormalizeColor(float* r, float* g, float* b, float* light) {
    uint16_t red, green, blue, light_raw;

    // Read raw data from the sensor
    tcs.setInterrupt(false);  // Turn on LED
    delay(50);
    tcs.getRawData(&red, &green, &blue, &light_raw);
    tcs.setInterrupt(true);   // Turn off LED

    // Store the light intensity value
    *light = (float)light_raw;

    // Normalize RGB values to [0, 255] range
    if (light_raw > 0) {
        *r = ((float)red / light_raw) * 255.0;
        *g = ((float)green / light_raw) * 255.0;
        *b = ((float)blue / light_raw) * 255.0;
    } else {
        *r = *g = *b = 0; // Avoid division by zero
    }

    // Debugging output (only if TEST_MODE is enabled)
    #if TEST_MODE
        Serial.print("Raw -> R: "); Serial.print(red);
        Serial.print(" G: "); Serial.print(green);
        Serial.print(" B: "); Serial.print(blue);
        Serial.print(" Light: "); Serial.println(light_raw);

        Serial.print("Normalized -> R: "); Serial.print(*r);
        Serial.print(" G: "); Serial.print(*g);
        Serial.print(" B: "); Serial.print(*b);
        Serial.print(" Light: "); Serial.println(*light);
    #endif
}
