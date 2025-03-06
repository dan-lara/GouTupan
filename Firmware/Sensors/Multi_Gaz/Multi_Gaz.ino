#include "Multi_Gaz.hpp"

void setup() 
{
    setupGasSensor();
}

void loop() 
{
    float nh3, co, no2, c3h8, c4h10, ch4, h2, c2h5oh;
    readGasValues(&nh3, &co, &no2, &c3h8, &c4h10, &ch4, &h2, &c2h5oh);
    delay(1000);
}