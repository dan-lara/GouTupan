// Read_and_Send.ino
#define TEST_MODE 0        // Set to 1 to enable Serial Monitor debugging, 0 to disable

#include <ArduinoLowPower.h> // Break low power mode

//LoRa
#include "LoRa_Transmission.hpp"

//CO2 + T/H
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
    //#include "DFRobot_SEN0308.hpp"
    //#define SENSOR_PIN A1   //30cm
    //#define SENSOR_PIN2 A2  //10cm
    //SoilMoistureSensor_SEN0308 cap;
    //SoilMoistureSensor_SEN0308 cap2;

//Soil Temperatur + Humidity
    #include "SHT3xSensor.hpp"
    SHT3xSensor SHT3xsensor(0x44, &Wire); // 使用默认地址0x44和Wire
    #include "DFRobot_SHT20.h"
    DFRobot_SHT20 sht20(&Wire, SHT20_I2C_ADDR);


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

//Timer
    #define DONE_PIN 3  // TPL5110 DONE 连接到 Arduino D2

// E-ink
    #include <SPI.h>
    #include "epd2in9b_V3.hpp"
    #include "epd_text.hpp"
    Epd epd;
    EpdText epdText(epd);

//NPK
    #include "NPK.hpp"

#define TEST_MODE 0        // Set to 1 to enable Serial Monitor debugging, 0 to disable
#define NB_MAX_SENSOR_ATTEMPT 10
long int t=0;

void setup() 
{
    delay(1000);

    #if TEST_MODE
      t = millis();  
      Serial.begin(115200);
      Serial.println("START");

    #endif
    
    //setup Timer
    pinMode(DONE_PIN, OUTPUT);
    digitalWrite(DONE_PIN, LOW); // 初始状态，DONE 置低
    

    // // E-ink
    // #if TEST_MODE
    // Serial.print("2.9inch b V3 e-Paper init \r\n ");
    // #endif
    // // if(flag == false){
    //   if (epd.Init() != 0) 
    //   {
    //       #if TEST_MODE
    //         Serial.print("e-Paper init failed\r\n ");
    //       #endif
    //       return;
    //   }
    // // }
    // epd.Clear();

// LoRa
    #if TEST_MODE
      Serial.println("Begining LoRa");
    #endif
    initializeLoRa();
    #if TEST_MODE
      Serial.println("LoRa Initialized");
    #endif

// Sensors Setup
    
    // CO2 + T/H
     int attempts = 0;
    while (!scd30.begin() && attempts < NB_MAX_SENSOR_ATTEMPT) 
    {
        #if TEST_MODE
            Serial.println("Tentative de connexion au capteur CO2...");
        #endif
        delay(400);  // Attendre 1 seconde avant de réessayer
        attempts++;
    }

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
    //attempts = 0;
    //while (cap2.begin(SENSOR_PIN2) && attempts < NB_MAX_SENSOR_ATTEMPT) 
    //{
    //    #if TEST_MODE
    //        Serial.println("Sensor initialized successfully.");
    //    #endif
    //    delay(400); 
    //    attempts++;
    //}

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

    attempts = 0;
    sht20.initSHT20();
    // while (!sht20.initSHT20() && attempts < NB_MAX_SENSOR_ATTEMPT) 
    // {
    //     #if TEST_MODE
    //         Serial.println("ERROR: SHT20 sensor not detected!");
    //     #endif
    //     delay(400); 
    //     attempts++;
    // }

    // Battery
    batman.begin(ANALOG_PIN, CHARGE_PIN, FAULT_PIN);
    setupGasSensor();

    // Air Quality
    airSensor.begin(NB_MAX_SENSOR_ATTEMPT);
    oxygen.setup_O2_sensor(NB_MAX_SENSOR_ATTEMPT);

    // // // E-ink
    #if TEST_MODE
    Serial.print("2.9inch b V3 e-Paper init \r\n ");
    #endif
    if (epd.Init() != 0) {          // Échec de l'initialisation (y compris le délai d'attente ou l'erreur matérielle)
        epd.eink_failed = true;     // Forcer l'indicateur de synchronisation
        #if TEST_MODE
            Serial.println("FAILED. Skipping E-ink.");
        #endif
    } else {
        #if TEST_MODE
            Serial.println("SUCCESS.");
        #endif
        //epd.Clear();               // Effacer l'écran uniquement en cas de succès
    }


    // NPK
    #if TEST_MODE
        Serial.println("Tentative de connexion au capteur NPK...");
    #endif
    Serial1.begin(9600);

    #if TEST_MODE
        Serial.print("Setup completed in: ");
        Serial.print(millis() - t);
        Serial.print(" ms ==> ");
        Serial.print((millis() - t) / 1000);
        Serial.println(" s");
        Serial.println("---------------------------------------------------------");
    #endif
    // pinMode(LED_BUILTIN, OUTPUT);
    delay(2000);
    // digitalWrite(LED_BUILTIN, HIGH);  // Liga o LED
    // delay(500);                       // Espera 500ms
    // digitalWrite(LED_BUILTIN, LOW);   // Desliga o LED
    // delay(500);
}

void loop() 
{
    // Example of sending temperature data (approximately 38.9°C)
    uint8_t mux_code = 0x1; // Sending 1 data
    float outside_temperature = -7.5865; // °C
    float outside_humidity = 49.85423; // %
    float outside_CO2 = 3220.53; // ppm

    // CO2 + T/H
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
    #if TEST_MODE
    {
      t = millis();
        Serial.println("==================================================================");
        Serial.println("==========================Test Mode===============================");
        Serial.print("Temperature: ");
        Serial.print(outside_temperature, 1);   // Only print one decimal place
        Serial.print("C");
        Serial.print(" Humidity: ");
        Serial.print(outside_humidity, 1);   // Only print one decimal place
        Serial.print("%");
        Serial.print(" CO2: ");
        Serial.print(outside_CO2, 1);   // Only print one decimal place
        Serial.println("ppm");
    }
    #endif
    // Battery

    //float voltage = batman.read(false);
    //bool charging = batman.isCharging();
    //bool fault = batman.isFault();

    //float battery_level = batman.read(); // % Voltage
    float battery_level = batman.read(true); // % Voltage
    #if TEST_MODE
      Serial.print("Battery Level:");
      Serial.print(battery_level, 1);
      Serial.println("%");
    #endif

    // Soil Nutrients
    float soil_nutrients_N_Nitrogen = 198.29; // ppm
    float soil_nutrients_P_Phosphorus = 13.22; // ppm
    float soil_nutrients_K_Potassium = 287.99; // ppm

    soil_nutrients_N_Nitrogen = read_NPK_N_Nitrogen();  // ppm
    soil_nutrients_P_Phosphorus = read_NPK_P_Phosphorus();  // ppm
    soil_nutrients_K_Potassium = read_NPK_K_Potassium();  // ppm

    #if TEST_MODE
        Serial.print("N = ");
        Serial.print(soil_nutrients_N_Nitrogen);
        Serial.print(" mg/kg  ");

        Serial.print("P = ");
        Serial.print(soil_nutrients_P_Phosphorus);
        Serial.print(" mg/kg  ");

        Serial.print("K = ");
        Serial.print(soil_nutrients_K_Potassium);
        Serial.println(" mg/kg  ");
    #endif
    

    // Soil Temperatur + Humidity
    //float surface_temperature = 8.58; // °C
    //float surface_humidity = cap2.read(); // %

    // SHT3x
    auto data_SHT3xsensor = SHT3xsensor.measure();
    float deep_temperature = data_SHT3xsensor.first;
    float deep_humidity = data_SHT3xsensor.second;
    // SHT20
    float surface_temperature = sht20.readTemperature();
    float surface_humidity = sht20.readHumidity();
    
    #if TEST_MODE
      Serial.print("SHT30 Temperature:");
      Serial.print(deep_temperature, 1);   // Only print one decimal place
      Serial.print("C");
      Serial.print(" SHT30 Humidity:");
      Serial.print(deep_humidity, 1);   // Only print one decimal place
      Serial.print("%");
      Serial.println();
      
      Serial.print("SHT20 Temperature:");
      Serial.print(surface_temperature, 1);   // Only print one decimal place
      Serial.print("C");
      Serial.print(" SHT20 Humidity:");
      Serial.print(surface_humidity, 1);   // Only print one decimal place
      Serial.print("%");
      Serial.println();
    #endif
    //RGB
    float R_RGB, G_RGB, B_RGB, light_intensity;

    readAndNormalizeColor(&R_RGB, &G_RGB, &B_RGB, &light_intensity);

    #if TEST_MODE
      Serial.println("TCS34725 RGB Color Sensor");
      Serial.print("R: ");
      Serial.print(R_RGB);
      Serial.print(" G: ");
      Serial.print(G_RGB);
      Serial.print(" B: ");
      Serial.print(B_RGB);
      Serial.print(" Light Intensity: ");
      Serial.println(light_intensity);
    #endif

    uint16_t light_infrared = 0;
    light_infrared = SI1145.readIR();
    float light_ultraviolet, light_intesity_SI1145;
    light_ultraviolet = (float)(SI1145.readUV())/100;
    light_intesity_SI1145 = (float)SI1145.readVisible();

    #if TEST_MODE
      Serial.println("SI1145 UV/IR Sensor");
      Serial.print("Vis: "); Serial.println(light_intesity_SI1145);
      Serial.print("IR: "); Serial.println(light_infrared);
      Serial.print("UV: "); Serial.println(light_ultraviolet);
    #endif

    float pressure = getPressure();
    float quality = airSensor.readAirQuality();
    float O2 = oxygen.getOxygenData(10);

    #if TEST_MODE
      Serial.print("Pressure : ");
      Serial.println(pressure);
      Serial.print("Air Quality (VOC) : ");
      Serial.println(quality);
      if (quality >= 0 && quality <= 100) {
        Serial.println("Air Quality: Excellent. No need to ventilate or purify.");
      } else if (quality > 100 && quality <= 200) {
        Serial.println("Air Quality: Good. No need to ventilate or purify.");
      } else if (quality > 200 && quality <= 400) {
        Serial.println("Air Quality: Moderate. Ventilate and purify.");
      } else if (quality > 400 && quality <= 500) {
        Serial.println("Air Quality: Poor. Ventilate and purify intensely.");
      } else {
        Serial.println("Air Quality: Out of range. Check sensor.");
      }
      Serial.print("O2 : ");
      Serial.print(O2);
      Serial.print(" ppm <==> ");
      Serial.print(O2/10000);
      Serial.println(" \%Vol");
    #endif

    float nh3, co, no2, c3h8, c4h10, ch4, h2, c2h5oh;
    readGasValues(&nh3, &co, &no2, &c3h8, &c4h10, &ch4, &h2, &c2h5oh);

    #if TEST_MODE
      Serial.print("NH3: ");
      Serial.print(nh3);
      Serial.print(" ppm");
      Serial.print(" CO: ");
      Serial.print(co);
      Serial.print(" ppm");
      Serial.print(" NO2: ");
      Serial.print(no2);
      Serial.print(" ppm");
      Serial.print(" C3H8: ");
      Serial.print(c3h8);
      Serial.println(" ppm");
      Serial.print("C4H10: ");
      Serial.print(c4h10);
      Serial.print(" ppm");
      Serial.print(" CH4: ");
      Serial.print(ch4);
      Serial.print(" ppm");
      Serial.print(" H2: ");
      Serial.print(h2);
      Serial.print(" ppm");
      Serial.print(" C2H5OH: ");
      Serial.print(c2h5oh);
      Serial.println(" ppm");
    #endif
    
    delay(2000);

      #if TEST_MODE
        Serial.println("Ready to send LoRa");
      #endif
      digitalWrite(LED_BUILTIN, HIGH);
        Send_LoRa_Data(mux_code, outside_temperature, outside_CO2, outside_humidity, battery_level,
                    soil_nutrients_N_Nitrogen, soil_nutrients_P_Phosphorus, soil_nutrients_K_Potassium,
                    surface_temperature, surface_humidity,
                    deep_temperature, deep_humidity,
                    light_intensity, light_infrared, light_ultraviolet,
                    R_RGB, G_RGB, B_RGB,
                    pressure, quality, O2,
                    nh3, co, no2, c3h8, c4h10, ch4, h2, c2h5oh);
      digitalWrite(LED_BUILTIN, LOW);
    delay(7500);
        #if TEST_MODE
          Serial.println("finished sending LoRa");
        #endif
    // E-ink display
    delay(500);
    if (!epd.eink_failed) {
    epdText.updateDisplay(mux_code, outside_temperature, outside_CO2, outside_humidity, battery_level,
                            soil_nutrients_N_Nitrogen, soil_nutrients_P_Phosphorus, soil_nutrients_K_Potassium,
                            surface_temperature, surface_humidity,
                            deep_temperature, deep_humidity,
                            light_intensity, light_infrared, light_ultraviolet,
                            R_RGB, G_RGB, B_RGB,
                            pressure, quality, O2,
                            nh3, co, no2, c3h8, c4h10, ch4, h2, c2h5oh);
        #if TEST_MODE
            Serial.println("E-ink updated successfully");
        #endif
    } else {
        #if TEST_MODE
            Serial.println("Skipping E-ink due to previous failure");
        #endif
    }
    
      #if TEST_MODE
        // Serial.println("Finished E-ink display");
        Serial.print("Processing time: ");
        Serial.print(millis()-t);
        Serial.print(" ms ==> ");
        Serial.print((millis()-t)/1000);
        Serial.println(" s");
        Serial.println("==================================================================");
        delay(5000);
      #endif

    // flag = false;//eink
    // delay(7500);  // Comply with transmission constraints (200,000 ms = 3.33 min) => ici toutes les 5 min
    digitalWrite(DONE_PIN, HIGH);
    delay(1);
    digitalWrite(DONE_PIN, LOW);      
    delay(1);
}
