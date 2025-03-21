#define TEST_MODE 1
#include "UV_IR_SI1145.hpp"

Adafruit_SI1145 uvSensor;

bool setupUVIRSensor() 
{
    if (!uvSensor.begin()) 
    {
        return false;  // Le capteur n'est pas détecté
    }

    #if TEST_MODE
        Serial.println("UV/IR sensor initialized successfully.");
    #endif

    return true;
}

void getUVIRData(float &visible, float &infrared, float &uv) 
{
    visible = uvSensor.readVisible();
    infrared = uvSensor.readIR();
    uv = uvSensor.readUV() / 100.0;  // Conversion de l'index UV

    #if TEST_MODE
        Serial.print("Debug - Visible: "); Serial.println(visible);
        Serial.print("Debug - IR: "); Serial.println(infrared);
        Serial.print("Debug - UV: "); Serial.println(uv);
    #endif
}
