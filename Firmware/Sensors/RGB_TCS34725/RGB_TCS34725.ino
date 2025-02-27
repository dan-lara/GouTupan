#include "ColorSensor.hpp"

#define TEST_MODE 1  // Set to 1 to enable debug output, 0 to disable

void setup() {
    Serial.begin(115200);
    while (!Serial);

    #if TEST_MODE
        Serial.println("Initializing color sensor...");
    #endif

    if (!initColorSensor()) {
        #if TEST_MODE
            Serial.println("ERROR: Color sensor not detected!");
        #endif
        while (1); // Stop execution if sensor is not detected
    }

    #if TEST_MODE
        Serial.println("Color sensor successfully initialized!");
    #endif
}

void loop() {
    float r, g, b, light;

    // Read and normalize color values
    readAndNormalizeColor(&r, &g, &b, &light);

    #if TEST_MODE
        Serial.print("R: "); Serial.print(r);
        Serial.print(" G: "); Serial.print(g);
        Serial.print(" B: "); Serial.print(b);
        Serial.print(" Light: "); Serial.println(light);
    #endif

    delay(1000); // Wait 1 second before the next reading
}
