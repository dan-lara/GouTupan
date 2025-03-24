#ifndef EPDIF_H
#define EPDIF_H

#include <Arduino.h>

// Pin definition
#define BUSY_PIN        4 // Purple (BUSY -> 4)
#define RST_PIN         5 // White  (RST -> 5)
#define DC_PIN          6 // Green  (DC -> 6)
#define CS_PIN          7 // Orange (CS -> 7)
#define PWR_PIN         8 // Blue   (DIN -> MOSI)
//                      9 // Yellow (CLK -> SCK)

class EpdIf {
public:
    EpdIf(void);
    ~EpdIf(void);

    static int  IfInit(void);
    static void DigitalWrite(int pin, int value); 
    static int  DigitalRead(int pin);
    static void DelayMs(unsigned int delaytime);
    static void SpiTransfer(unsigned char data);
};

#endif
