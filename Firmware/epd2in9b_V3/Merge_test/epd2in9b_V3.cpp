#include <stdlib.h>
#include "epd2in9b_V3.hpp"

#define TEST_MODE 1

Epd::~Epd() {
};

Epd::Epd() {
    reset_pin = RST_PIN;
    dc_pin = DC_PIN;
    cs_pin = CS_PIN;
    busy_pin = BUSY_PIN;
    width = EPD_WIDTH / 8;
    height = EPD_HEIGHT;
};

int Epd::Init(void) {
    if (IfInit() != 0) {
        return -1;
    }
    Reset();

    SendCommand(0x04);  // power on
    WaitUntilIdle();    // waiting for the electronic paper IC to release the idle signal

    SendCommand(0x00);  // panel setting
    SendData(0x0f); // default data
    SendData(0x89); // 128x296,Temperature sensor, boost and other related timing settings

    SendCommand(0x61);  // Display resolution setting
    SendData (0x80);
    SendData (0x01);
    SendData (0x28);

    SendCommand(0X50);  // VCOM AND DATA INTERVAL SETTING      
    SendData(0x77); // WBmode:VBDF 17|D7 VBDW 97 VBDB 57   
                    // WBRmode:VBDF F7 VBDW 77 VBDB 37  VBDR B7
    return 0;
}

/**
 *  @brief: basic function for sending commands
 */
void Epd::SendCommand(unsigned char command) {
    DigitalWrite(dc_pin, LOW);
    SpiTransfer(command);
}

/**
 *  @brief: basic function for sending data
 */
void Epd::SendData(unsigned char data) {
    DigitalWrite(dc_pin, HIGH);
    SpiTransfer(data);
}

/**
 *  @brief: Wait until the busy_pin goes HIGH
 */
/*
void Epd::WaitUntilIdle(void) {
  unsigned char busy;
  Serial.print("e-Paper busy \r\n ");
  do
  {
    SendCommand(0x71);
    busy = DigitalRead(busy_pin);
    busy =!(busy & 0x01);        
  }
  while(busy); 
  Serial.print("e-Paper busy release \r\n ");
  DelayMs(200);
}
*/
void Epd::WaitUntilIdle(void) {
    unsigned char busy;
    unsigned long start = millis();
    #if TEST_MODE
        Serial.println("e-Paper busy...");
    #endif

    do {
        SendCommand(0x71);                    // 查询状态
        busy = DigitalRead(busy_pin);         // 读取 busy 引脚
        busy = !(busy & 0x01);                // 转换为逻辑值：1=忙，0=就绪

        if (millis() - start > 15000) {        // 超时(15s)处理
            #if TEST_MODE
                Serial.println("e-Paper busy timeout! Forcing reset...");
            #endif
            Reset();                          // 硬件复位
            Init();                           // 重新初始化配置
            break;
        }

        DelayMs(100);                         // 适当降低查询频率
    } while (busy);

    #if TEST_MODE
        Serial.println("e-Paper ready.");
    #endif
    DelayMs(200);                             // 确保状态稳定
}

/**
 *  @brief: module reset.
 *          often used to awaken the module in deep sleep,
 *          see Epd::Sleep();
 */
void Epd::Reset(void) {
    DigitalWrite(reset_pin, HIGH);
    DelayMs(200);   
    DigitalWrite(reset_pin, LOW);                // module reset    
    DelayMs(5);
    DigitalWrite(reset_pin, HIGH);
    DelayMs(200);    
}

void Epd::DisplayFrame(const UBYTE *blackimage, const UBYTE *ryimage) {
    SendCommand(0x10);  // 黑白图层写入
    for (UWORD j = 0; j < height; j++) {
        for (UWORD i = 0; i < width; i++) {
          SendData(pgm_read_byte(&blackimage[i + (j*width)]));
        }
    }
    SendCommand(0x92);
    
    SendCommand(0x13);  // 红色图层写入
    for (UWORD j = 0; j < height; j++) {
        for (UWORD i = 0; i < width; i++) {
          SendData(pgm_read_byte(&ryimage[i + (j*width)]));
        }
    }
    SendCommand(0x92);

    SendCommand(0x12);  // 刷新屏幕命令
    WaitUntilIdle();
}

void Epd::Clear(void) {
    //send black data
    SendCommand(0x10);
    for (UWORD j = 0; j < height; j++) {
        for (UWORD i = 0; i < width; i++) {
            SendData(0xff);
        }
    }

    //send red data
    SendCommand(0x13);
    for (UWORD j = 0; j < height; j++) {
        for (UWORD i = 0; i < width; i++) {
            SendData(0xff);
        }
    }
    
    SendCommand(0x12);
    WaitUntilIdle();

}

/**
 *  @brief: After this command is transmitted, the chip would enter the 
 *          deep-sleep mode to save power. 
 *          The deep sleep mode would return to standby by hardware reset. 
 *          The only one parameter is a check code, the command would be
 *          You can use EPD_Reset() to awaken
 */
void Epd::Sleep(void) {
    SendCommand(0x02); // POWER_OFF
    WaitUntilIdle();
    SendCommand(0x07); // DEEP_SLEEP
    SendData(0xA5); // check code
}



/* END OF FILE */
