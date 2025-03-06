// Read_and_Send.ino

#define TEST_MODE 0        // Set to 1 to enable Serial Monitor debugging, 0 to disable

#include "LoRa_Transmission.hpp"
#include <ArduinoLowPower.h> // Break low power mode
#include <Adafruit_SCD30.h>
Adafruit_SCD30  scd30;

// Sensor's Include
#include "Baroleter_High_Accuracy.hpp"
#include "ColorSensor.hpp"

#include "DFRobot_SEN0308.hpp"
#define SENSOR_PIN A1   //30cm
#define SENSOR_PIN2 A2  //10cm
SoilMoistureSensor_SEN0308 cap;
SoilMoistureSensor_SEN0308 cap2;

#include "BatteryManager.hpp"
#define ANALOG_PIN A3
#define CHARGE_PIN 7
#define FAULT_PIN 6
BatteryManager batman;

void setup() 
{
    if (!scd30.begin()) 
        {
            while (1) { delay(1000); }
        }
    initializeLoRa();

    // Sensors Setup
    
    setupBarometer(); 
    float pressure_test = getPressure();

    if (!initColorSensor()) 
    {
        #if TEST_MODE
            Serial.println("ERROR: Color sensor not detected!");
        #endif
        while (1); // Stop execution if sensor is not detected
    }

    if (cap.begin(SENSOR_PIN)) 
    {
        #if TEST_MODE
            Serial.println("Sensor initialized successfully.");
        #endif
    } 
    else 
    {
        #if TEST_MODE
            Serial.println("Failed to initialize sensor.");
        #endif
        while (1);
    }
    if (cap2.begin(SENSOR_PIN2)) 
    {
        #if TEST_MODE
            Serial.println("Sensor initialized successfully.");
        #endif
    } 
    else 
    {
        #if TEST_MODE
            Serial.println("Failed to initialize sensor.");
            while (1);
        #endif
    }

    batman.begin(ANALOG_PIN, CHARGE_PIN, FAULT_PIN);

    delay(2000);
}

void loop() 
{
    // Example of sending temperature data (approximately 38.9°C)
    uint8_t mux_code = 0x1; // Sending 1 data
    float outside_temperature = -7.5865; // °C
    float outside_humidity = 49.85423; // %
    float outside_CO2 = 3220.53; // ppm

    if (scd30.dataReady()) 
    {
        if (scd30.read()) 
        {
            // Mise à jour des valeurs avec celles lues du capteur
            outside_temperature = scd30.temperature;
            outside_humidity = scd30.relative_humidity;
            outside_CO2 = scd30.CO2;
        }
    }

    //float voltage = batman.read(false);
    //bool charging = batman.isCharging();
    //bool fault = batman.isFault();

    //float battery_level = batman.read(); // % Voltage
    float battery_level = batman.read(false)*10; // % Voltage

    float soil_nutrients_N_Nitrogen = 198.29; // ppm
    float soil_nutrients_P_Phosphorus = 13.22; // ppm
    float soil_nutrients_K_Potassium = 287.99; // ppm

    float surface_temperature = 8.58; // °C
    float surface_humidity = cap2.read(); // %

    float deep_temperature = 7.22; // °C
    float deep_humidity = cap.read(); // %

    float R_RGB, G_RGB, B_RGB, light_intensity;

    readAndNormalizeColor(&R_RGB, &G_RGB, &B_RGB, &light_intensity);

    float light_infrared = 928.13; // Lux
    float light_ultraviolet = 1329.58; // Lux

    float pressure = getPressure();
    
    delay(2000);

/*
    #if TEST_MODE
        Serial.print("Pressure: ");
        Serial.print(pressure);
        Serial.println(" hPa");
    #endif
*/
    //bool Failure_Check = retrySendingStoredPayload(); // Check for sending failure
    // Real mode (online)
    //if (Failure_Check)
    //{
        Send_LoRa_Data(mux_code, outside_temperature, outside_CO2, outside_humidity, battery_level,
                    soil_nutrients_N_Nitrogen, soil_nutrients_P_Phosphorus, soil_nutrients_K_Potassium,
                    surface_temperature, surface_humidity,
                    deep_temperature, deep_humidity,
                    light_intensity, light_infrared, light_ultraviolet,
                    R_RGB, G_RGB, B_RGB,
                    pressure);
    //}
    // LowPower.sleep(20000);  // Put the microcontroller into sleep mode for 20 seconds BUT NEEDS a wake-up module or RTC timer
    delay(60000);  // Comply with transmission constraints (200,000 ms = 3.33 min) => ici toutes les 5 min
        
}