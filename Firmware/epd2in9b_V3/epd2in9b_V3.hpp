#ifndef EPD2IN9B_V3_H
#define EPD2IN9B_V3_H

#include "epdif.hpp"

// Display resolution
#define EPD_WIDTH       128
#define EPD_HEIGHT      296

#define UWORD  unsigned int
#define UBYTE  unsigned char

class Epd : EpdIf {
public:
    Epd();
    ~Epd();
    int  Init(void);
    void WaitUntilIdle(void);
    void Reset(void);
    void DisplayFrame(const UBYTE *blackimage, const UBYTE *ryimage);
    void SendCommand(unsigned char command);
    void SendData(unsigned char data);
    void Sleep(void);
    void Clear(void);
    
private:
    unsigned int reset_pin;
    unsigned int dc_pin;
    unsigned int cs_pin;
    unsigned int busy_pin;
    unsigned long width;
    unsigned long height;
};

#endif

/* END OF FILE */
