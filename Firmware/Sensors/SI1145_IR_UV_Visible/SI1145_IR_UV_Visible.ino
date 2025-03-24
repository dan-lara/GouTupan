#include <Wire.h>
#include "SI1145_IR_UV_Visible.hpp"

SI1145_IR_UV_Visible sensor;

void setup() 
{
    #if TEST_MODE
        Serial.begin(115200);
        Serial.println("Beginning Si1145!");
    #endif 
    
    sensor.begin();
        delay(1000);
}

void loop() 
{
    uint16_t vis = sensor.readVisible();
    uint16_t ir = sensor.readIR();
    float uv = sensor.readUV();
    #if TEST_MODE
        Serial.print("Lumière visible (raw) : "); 
        Serial.println(vis);
        Serial.print("Lumière IR (raw)      : "); 
        Serial.println(ir);
        Serial.print("Indice UV            : "); 
        Serial.println(uv);
    #endif 
    delay(1000);
}
