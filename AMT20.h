#include <SPI.h>
#include <stdint.h>

// System will send 0xA5 when there is no data left to read or is waiting for conversion

// Read Delay
#define AMT20_RDELAY 30

// Commands for AMT20
#define AMT20_IDLE 0x00
#define AMT20_POS 0x10
#define AMT20_ZEROSET 0x70
#define AMT20_ZERODONE 0x80

// SPI settings
#define AMT20_BUS_SPEED 1L*1000*1000

class AMT20 {
  public:
  AMT20(SPIClass& AMT20_SPI_BUS, uint8_t AMT20_CS);

  void init();
  void read();
  void zero();
  
  uint16_t raw_value();
  float    angle();

  private:
  SPIClass& _SPI_BUS;
  uint8_t   _CS;

  uint16_t _encoder_raw;
  float    _encoder_angle;
  uint8_t  _encoder_data[2];
  
  uint8_t send_command(uint8_t command);
  
};

