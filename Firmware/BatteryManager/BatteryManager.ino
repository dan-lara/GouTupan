#include "BatteryManager.hpp"
#include <Arduino.h>

#define ANALOG_PIN A1
#define CHARGE_PIN 7
#define FAULT_PIN 6

BatteryManager batman;

void setup() {
    Serial.begin(9600);
    batman.begin(ANALOG_PIN, CHARGE_PIN, FAULT_PIN);
}

void loop() {
    float voltage = batman.read(false);
    bool charging = batman.isCharging();
    bool fault = batman.isFault();
    
    Serial.println("____________________");
    Serial.print("Voltage: ");
    Serial.print(voltage);
    Serial.println(" V");

    voltage = batman.getPercentage(); // Percentage
    Serial.print("Percentage: ");
    Serial.print(voltage);
    Serial.println("%");

    Serial.print("Charging: ");
    Serial.println(charging);

    Serial.print("Fault: ");
    Serial.println(fault);

    delay(3000); // Wait for 1 second before repeating
}