#ifndef __DFRobot_OxygenSensor_H__
#define __DFRobot_OxygenSensor_H__

#include <Arduino.h>
#include <Wire.h>

#define TEST_MODE 0  // Enable (1) or disable (0) debug prints

#define ADDRESS_0   0x70
#define ADDRESS_1   0x71
#define ADDRESS_2   0x72
#define ADDRESS_3   0x73  ///< i2c slave Address select
#define OCOUNT      100   ///< oxygen Count Value

#define Oxygen_IICAddress ADDRESS_3
#define OXYGEN_DATA_REGISTER 0x03   ///< register for oxygen data
#define USER_SET_REGISTER    0x08   ///< register for users to configure key value manually
#define AUTUAL_SET_REGISTER  0x09   ///< register that automatically configure key value
#define GET_KEY_REGISTER     0x0A   ///< register for obtaining key value

class DFRobot_OxygenSensor
{
public:
  DFRobot_OxygenSensor(TwoWire *pWire = &Wire);
  ~DFRobot_OxygenSensor();

  void setup_O2_sensor(uint8_t attempts);
  bool begin(uint8_t addr = Oxygen_IICAddress);
  void calibrate(float vol, float mv = 0);
  float getOxygenData(uint8_t collectNum);

private:
  void readFlash();
  void i2cWrite(uint8_t reg, uint8_t data);
  float getAverageNum(float bArray[], uint8_t len);

  uint8_t _addr;                               
  float _Key = 0.0;                          
  float oxygenData[OCOUNT] = {0.0};
  TwoWire *_pWire;
};
#endif