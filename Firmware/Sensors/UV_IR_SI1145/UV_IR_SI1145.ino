#include "UV_IR_SI1145.hpp"
#define TEST_MODE 1

void setup() 
{
    #if TEST_MODE
        Serial.begin(115200);
    #endif

    if (!setupUVIRSensor()) 
    {
        #if TEST_MODE
            Serial.println("UV/IR sensor not found!");
        #endif
        while (1); // Stop si le capteur n'est pas détecté
    }
}

void loop() 
{
    float visible, infrared, uv;
    getUVIRData(visible, infrared, uv);

    #if TEST_MODE
        Serial.println();
        Serial.print("Visible: "); Serial.print(visible);
        Serial.print("  IR: "); Serial.print(infrared);
        Serial.print("  UV Index: "); Serial.println(uv);
    #endif

    delay(3000);
}
