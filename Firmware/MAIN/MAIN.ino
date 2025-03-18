// Read_and_Send.ino

/* 
 * The sensor which aren't well fonctinned:
 *  Temperature, 
 *
 */

#define TEST_MODE 0        // Set to 1 to enable Serial Monitor debugging, 0 to disable

#include "LoRa_Transmission.hpp"
#include <ArduinoLowPower.h> // Break low power mode
#include <Adafruit_SCD30.h>
Adafruit_SCD30  scd30;
//Pressure
    #include "Baroleter_High_Accuracy.hpp"

//RGB
    #include "ColorSensor.hpp"

//Pressure
    #include "Baroleter_High_Accuracy.hpp"

//RGB
    #include "ColorSensor.hpp"

//Humidity soil
    #include "DFRobot_SEN0308.hpp"
    //#define SENSOR_PIN A1   //30cm
    #define SENSOR_PIN2 A2  //10cm
    //SoilMoistureSensor_SEN0308 cap;
    SoilMoistureSensor_SEN0308 cap2;
//Humidity soil
    #include "DFRobot_SEN0308.hpp"
    //#define SENSOR_PIN A1   //30cm
    #define SENSOR_PIN2 A2  //10cm
    //SoilMoistureSensor_SEN0308 cap;
    SoilMoistureSensor_SEN0308 cap2;

//Soil Temperatur + Humidity
    #include "SHT3xSensor.hpp"
    SHT3xSensor SHT3xsensor(0x44, &Wire); // 使用默认地址0x44和Wire

//Battery
    #include "BatteryManager.hpp"
    #define ANALOG_PIN A3
    #define CHARGE_PIN 7
    #define FAULT_PIN 6
    BatteryManager batman;

//Multi GAZ
    #include "Multi_Gaz.hpp"

//UV + IR 
    #include "SI114X.hpp"
    Adafruit_SI1145 SI1145 = Adafruit_SI1145();


//Air Quality
    #include "SEN0394_Air_Quality.hpp"
    AirSensor airSensor;

//O2 Sensor
    #include "SEN0332_O2.hpp"
    DFRobot_OxygenSensor oxygen;

#define TEST_MODE 0 
#define NB_MAX_SENSOR_ATTEMPT 10

void setup() 
{
    #if TEST_MODE
        Serial.begin(115200);
        Serial.println("START");
    #endif

    int attempts = 0;
    while (!scd30.begin() && attempts < NB_MAX_SENSOR_ATTEMPT) 
    {
        #if TEST_MODE
            Serial.println("Tentative de connexion au capteur CO2...");
        #endif
        delay(400);  // Attendre 1 seconde avant de réessayer
        attempts++;
    }
    #if TEST_MODE
        Serial.begin(115200);
        Serial.println("START");
    #endif

    int attempts = 0;
    while (!scd30.begin() && attempts < NB_MAX_SENSOR_ATTEMPT) 
    {
        #if TEST_MODE
            Serial.println("Tentative de connexion au capteur CO2...");
        #endif
        delay(400);  // Attendre 1 seconde avant de réessayer
        attempts++;
    }
    initializeLoRa();

    // Sensors Setup
    
    // Pressure
    setupBarometer(); 
    float pressure_test = getPressure();

    // RGB
    attempts = 0;
    while (!initColorSensor() && attempts < NB_MAX_SENSOR_ATTEMPT) 
    {
        #if TEST_MODE
            Serial.println("ERROR: Color sensor not detected!");
        #endif
        delay(400); 
        attempts++;
    }

    // Humidity soil
    attempts = 0;
    while (cap2.begin(SENSOR_PIN2) && attempts < NB_MAX_SENSOR_ATTEMPT) 
    {
        #if TEST_MODE
            Serial.println("Sensor initialized successfully.");
        #endif
        delay(400); 
        attempts++;
    }

    // UV + IR
    attempts = 0;
    while (!SI1145.begin() && attempts < NB_MAX_SENSOR_ATTEMPT) 
    {
        #if TEST_MODE
            Serial.println("ERROR: Si1145 is not detected!");
        #endif
        delay(400); 
        attempts++;
    }

    // Soil Temperatur + Humidity
    attempts = 0;
    while (!SHT3xsensor.begin() && attempts < NB_MAX_SENSOR_ATTEMPT) 
    {
        #if TEST_MODE
            Serial.println("ERROR: SHT3x sensor not detected!");
        #endif
        delay(400); 
        attempts++;
    }

    // Battery
    batman.begin(ANALOG_PIN, CHARGE_PIN, FAULT_PIN);
    setupGasSensor();

    // Air Quality
    airSensor.begin(NB_MAX_SENSOR_ATTEMPT);
    oxygen.setup_O2_sensor(NB_MAX_SENSOR_ATTEMPT);

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

    /*
    float deep_temperature = SHT3xsensor.temperature(); // °C
    float deep_humidity = SHT3xsensor.humidity(); //%
    */
    auto data_SHT3xsensor = SHT3xsensor.measure();
    float deep_temperature = data_SHT3xsensor.first;
    float deep_humidity = data_SHT3xsensor.second;


    float R_RGB, G_RGB, B_RGB, light_intensity;

    readAndNormalizeColor(&R_RGB, &G_RGB, &B_RGB, &light_intensity);

    uint16_t light_infrared = SI1145.readIR();
    Serial.print("Vis: "); Serial.println(SI1145.readVisible());
    Serial.print("IR: "); Serial.println(light_infrared);
    float light_ultraviolet = (float)(SI1145.readUV()) / 100;

    float pressure = getPressure();
    float quality = airSensor.readAirQuality();
    float O2 = oxygen.getOxygenData(10);

    float nh3, co, no2, c3h8, c4h10, ch4, h2, c2h5oh;
    readGasValues(&nh3, &co, &no2, &c3h8, &c4h10, &ch4, &h2, &c2h5oh);
    
    
    delay(2300);

        Send_LoRa_Data(mux_code, outside_temperature, outside_CO2, outside_humidity, battery_level,
                    soil_nutrients_N_Nitrogen, soil_nutrients_P_Phosphorus, soil_nutrients_K_Potassium,
                    surface_temperature, surface_humidity,
                    deep_temperature, deep_humidity,
                    light_intensity, light_infrared, light_ultraviolet,
                    R_RGB, G_RGB, B_RGB,
                    pressure, quality, O2,
                    nh3, co, no2, c3h8, c4h10, ch4, h2, c2h5oh);

    delay(7700);  // Comply with transmission constraints (200,000 ms = 3.33 min) => ici toutes les 5 min
        
}