#ifndef ARDUINO_KEY
#define ARDUINO_KEY

#define MODULE 1  // Define module selection
#define APPEUI "XXXXXXXXXXXXXXXX"

#if MODULE == 1  

    #define APPKEY "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"

#elif MODULE == 2  
    #define APPKEY "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"

#elif MODULE == 3
    #define APPKEY "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"

#else
    #error "Invalid MODULE selected"
#endif  

#endif  // ARDUINO_KEY