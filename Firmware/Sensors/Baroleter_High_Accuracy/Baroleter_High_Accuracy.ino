#include "Baroleter_High_Accuracy.hpp"

HP20x barometer;  // Create sensor object

void setup() 
{
    Serial.begin(115200);
    while (!Serial);  // Wait for serial connection

    barometer.begin();

    #if TEST_MODE
        Serial.println("Sensor initialized successfully.");
    #endif
}

void loop() 
{
    float pressure = barometer.readPressure();

    #if TEST_MODE
        Serial.print("Pressure: ");
        Serial.print(pressure);
        Serial.println(" hPa");
    #endif

    delay(1000);
}
