#ifndef ARDUINO_KEY
#define ARDUINO_KEY

#define MODULE 1  // Define module selection
#define APPEUI "AA2025AAAA2025AA"

#if MODULE == 1 

    #define APPKEY "7E72F8E351AA2605A8F71F03CE6835DC"

#elif MODULE == 2  
    #define APPKEY "F48978BC2A48520C22CE59A01DA1E5A8"

#elif MODULE == 3
    #define APPKEY "00000000000000000000000000000000"

#else
    #error "Invalid MODULE selected"
#endif  

#endif  // ARDUINO_KEY
