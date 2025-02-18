function decodeUplink(input) 
{
    let mux_code = input.bytes[0] >> 4;
    
    // Associer un nom au mux_code
    let mux_label = "";
    switch (mux_code) {
        case 1:
            mux_label = "Reading_1_data";
            let outside_Temperature = (((input.bytes[0] & 0x0F) << 4) | ((input.bytes[1] >> 4))) / 5;
            
            let outside_CO2 = ((input.bytes[1] & 0x0F) << 8) + (input.bytes[2] );  // Reconvertir le CO2
            let outside_Humidity = (((input.bytes[3]) << 4) + (input.bytes[4] & 0xF0 ))/25;  // Converting back humidity 
            
            let baterry_level = (((input.bytes[4] & 0x0F) << 4) | ((input.bytes[5] >> 4))) /2.5;
            
            let soil_nutrients_N_Nitrogen = (((input.bytes[5] & 0x0F) << 8) + (input.bytes[6]));
            let soil_nutrients_P_Phosphorus = (((input.bytes[7] & 0x0F) << 8) + (((input.bytes[8]) & 0xF0) >> 4));
            let soil_nutrients_K_Potassium = (((input.bytes[8] & 0x0F) << 8) + input.bytes[9]);

            
            let surface_temperature = (((input.bytes[10]) << 4) + (((input.bytes[11] & 0xF0)>> 4)))/25; 
            let surface_humidity = ((((input.bytes[11] & 0x0F)) << 8) + ((input.bytes[12])))/25;
            
            let deep_temperature = (((input.bytes[13]) << 4 )+ ((input.bytes[14] & 0xF0)>> 4))/25; 
            let deep_humidity = ((((input.bytes[14] & 0x0F)) << 8) + ((input.bytes[15])))/25;
            
            let light_intensity = (((input.bytes[16]) << 4 )+ ((input.bytes[17] & 0xF0)>> 4))*10; 
             
             
            return {
              data: {
                  Mux_Code: mux_label,  // Nom du Mux_Code
                  
                  Outside_Temperature: outside_Temperature,
                  Outside_CO2: outside_CO2,
                  Outside_Humidity: outside_Humidity,
                  
                  Baterry_Level: baterry_level,
                  
                  Soil_Nutrients_N_Nitrogen: soil_nutrients_N_Nitrogen,
                  Soil_Nutrients_P_Phosphorus: soil_nutrients_P_Phosphorus,
                  Soil_Nutrients_K_Potassium: soil_nutrients_K_Potassium,
                  
                  Surface_Temperature: surface_temperature, // Temperature in the soil surface (5-10cm)
                  Surface_Humidity: surface_humidity,
                  
                  Deep_Temperature: deep_temperature,       // Temperature at 30cm depth in soil
                  Deep_Humidity: deep_humidity,

                  Light_Intensity: light_intensity,
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
            Mux_Code: mux_label,  // Nom du Mux_Code
        }
    };
}