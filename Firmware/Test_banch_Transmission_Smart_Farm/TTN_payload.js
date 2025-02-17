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
             
            return {
              data: {
                  Mux_Code: mux_label,  // Nom du Mux_Code
                  Outside_Temperature: outside_Temperature,
                  Outside_CO2: outside_CO2,
                  
                  //Surface_Temperature:  surfaceTemperature,  // Temperature in the soil surface (5cm)
                  //Deep_Temperature : deepTemperature         // Temperature at 30cm depth in soil
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