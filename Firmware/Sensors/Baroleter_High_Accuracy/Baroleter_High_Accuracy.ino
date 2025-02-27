#include "Baroleter_High_Accuracy.hpp"
#define TEST_MODE 1

void setup() 
{
    setupBarometer();
}

void loop() 
{
    float pressure = getPressure();

    #if TEST_MODE
        Serial.print("Pressure: ");
        Serial.print(pressure);
        Serial.println(" hPa");
    #endif

    delay(1000);
}
