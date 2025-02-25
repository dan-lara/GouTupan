function decodeUplink(input) 
{
    let mux_code = input.bytes[0] >> 4;
    
    // Assign a name to the mux_code
    let mux_label = "";
    switch (mux_code) {
        case 1:
            mux_label = "Reading_1_data";
            let outside_temperature = (input.bytes[17] << 8) | (input.bytes[18]);

            // Check if the number is negative
            if (outside_temperature & 0x8000) {  // If the most significant bit is 1 (negative number)
                outside_temperature -= 0x10000; // Convert to signed number
            }
            
            outside_Temperature = outside_temperature / 100;  // Divide to get the temperature in degrees
            
            let outside_CO2 = ((input.bytes[0] & 0x0F) << 8) + (input.bytes[1]);  // Convert CO2 back
            let outside_Humidity = (((input.bytes[2]) << 4) + (input.bytes[3] & 0xF0 ) >> 4 ) / 40;  // Convert humidity back 
            
            let battery_level = (((input.bytes[3] & 0x0F) << 4) | ((input.bytes[4] >> 4))) / 2.5;
            
            let soil_nutrients_N_Nitrogen = (((input.bytes[4] & 0x0F) << 8) + (input.bytes[5]));
            let soil_nutrients_P_Phosphorus = (((input.bytes[6] & 0x0F) << 8) + (((input.bytes[7]) & 0xF0) >> 4));
            let soil_nutrients_K_Potassium = (((input.bytes[7] & 0x0F) << 8) + input.bytes[8]);

            
            let surface_temperature = (((input.bytes[9]) << 4) + (((input.bytes[10] & 0xF0) >> 4))) / 40; 
            let surface_humidity = ((((input.bytes[10] & 0x0F)) << 8) + ((input.bytes[11]))) / 40;
            
            let deep_temperature = (((input.bytes[12]) << 4 ) + ((input.bytes[13] & 0xF0) >> 4)) / 40; 
            let deep_humidity = ((((input.bytes[13] & 0x0F)) << 8) + ((input.bytes[14]))) / 40;
            
            
            let light_intensity = (((input.bytes[15]) << 4 ) + ((input.bytes[16] & 0xF0) >> 4)) * 10; 
            let light_infrared  = (((input.bytes[19]) << 4 ) + ((input.bytes[20] & 0xF0) >> 4)) * 10; 
            let light_ultraviolet  = ((((input.bytes[20]) & 0x0F) << 8 ) + ((input.bytes[21]))) * 10; 
            
            let r_RGB = (input.bytes[22]);
            let g_RGB = (input.bytes[23]);
            let b_RGB = (input.bytes[24]); 

            let pressure = (((input.bytes[25]) << 4 ) + ((input.bytes[26] & 0xF0) >> 4)) / 3; 

            return {
              data: {
                    Mux_Code: mux_label,  // Mux_Code name
                    
                    Outside_Temperature: outside_Temperature,
                    Outside_CO2: outside_CO2,
                    Outside_Humidity: outside_Humidity,
                    
                    Battery_Level: battery_level,
                    
                    Soil_Nutrients_N_Nitrogen: soil_nutrients_N_Nitrogen,
                    Soil_Nutrients_P_Phosphorus: soil_nutrients_P_Phosphorus,
                    Soil_Nutrients_K_Potassium: soil_nutrients_K_Potassium,
                    
                    Surface_Temperature: surface_temperature, // Temperature in the soil surface (5-10cm)
                    Surface_Humidity: surface_humidity,
                    
                    Deep_Temperature: deep_temperature,       // Temperature at 30cm depth in soil
                    Deep_Humidity: deep_humidity,

                    Light_Intensity: light_intensity,
                    Light_Infrared: light_infrared,
                    Light_Ultraviolet: light_ultraviolet,

                    RGB_R : r_RGB,
                    RGB_G : g_RGB,
                    RGB_B : b_RGB,

                    Pressure : pressure,
                  }
            }
            break;
        case 2:
            mux_label = "Reading_2_data";
            break;
        case 3:
            mux_label = "Message";
            break;
        default:
            mux_label = "Unknown_Mux_Code";
    }

    return {
        data: {
            Mux_Code: mux_label,  // Mux_Code name
        }
    };
}
