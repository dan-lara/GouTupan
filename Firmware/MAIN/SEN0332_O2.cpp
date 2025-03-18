#include "SEN0332_O2.hpp"

DFRobot_OxygenSensor::DFRobot_OxygenSensor(TwoWire *pWire) {
  this->_pWire = pWire;
}

DFRobot_OxygenSensor::~DFRobot_OxygenSensor() {
  this->_pWire = NULL;
}

void DFRobot_OxygenSensor::setup_O2_sensor(uint8_t attempts) 
{
    while (attempts-- > 0 && !begin(Oxygen_IICAddress)) { // ✅ Correction : utiliser `begin()` directement
        delay(1000); 
        #if TEST_MODE 
            Serial.println("I2C device not found, retrying...");
        #endif
    }
    #if TEST_MODE 
        Serial.println("I2C device initialized!");
    #endif
}

bool DFRobot_OxygenSensor::begin(uint8_t addr) {
  this->_addr = addr;
  _pWire->begin();
  _pWire->beginTransmission(_addr);
  if (_pWire->endTransmission() == 0) {
    return true;
  }
  return false;
}

void DFRobot_OxygenSensor::readFlash() {
  uint8_t value = 0;
  _pWire->beginTransmission(_addr);
  _pWire->write(GET_KEY_REGISTER);
  _pWire->endTransmission();
  delay(50);
  _pWire->requestFrom(_addr, (uint8_t)1);
  
  while (_pWire->available()) {
    value = _pWire->read();
  }
  
  this->_Key = (value == 0) ? (20.9 / 120.0) : ((float)value / 1000.0);
}

void DFRobot_OxygenSensor::i2cWrite(uint8_t reg, uint8_t data) {
  _pWire->beginTransmission(_addr);
  _pWire->write(reg);
  _pWire->write(data);
  _pWire->endTransmission();
}

void DFRobot_OxygenSensor::calibrate(float vol, float mv) {
  uint8_t keyValue = (mv < 0.000001 && mv > -0.000001) ? vol * 10 : (vol / mv) * 1000;
  i2cWrite((mv < 0.000001 && mv > -0.000001) ? USER_SET_REGISTER : AUTUAL_SET_REGISTER, keyValue);
}

float DFRobot_OxygenSensor::getOxygenData(uint8_t collectNum) {
  if (collectNum == 0) return -1.0;

  uint8_t rxbuf[3] = {0}, k = 0;
  static uint8_t i = 0;

  readFlash();

  // Décalage des anciennes données
  for (uint8_t j = collectNum - 1; j > 0; j--) {
    oxygenData[j] = oxygenData[j - 1];
  }

  _pWire->beginTransmission(_addr);
  _pWire->write(OXYGEN_DATA_REGISTER);
  _pWire->endTransmission();
  delay(100);
  _pWire->requestFrom(_addr, (uint8_t)3);

  while (_pWire->available()) {
    rxbuf[k++] = _pWire->read();
  }

  oxygenData[0] = (_Key) * (rxbuf[0] + (rxbuf[1] / 10.0) + (rxbuf[2] / 100.0));
  
  if (i < collectNum) i++;

  return getAverageNum(oxygenData, i)*10000;
}

float DFRobot_OxygenSensor::getAverageNum(float bArray[], uint8_t len) {
  if (len == 0) return 0.0;

  double sum = 0.0;
  for (uint8_t i = 0; i < len; i++) {
    sum += bArray[i];
  }
  return sum / (float)len;
}
