#include <SPI.h>
#include <stdint.h>

// SPI settings
#define AMT20_BUS_SPEED 1L*1000*1000

class AMT20 {
  public:
  AMT20(SPIClass& AMT20_SPI_BUS, uint8_t AMT20_CS);

  void init();
  bool read();
  bool zero();
  
  uint16_t raw_value();
  float    angle();

  private:
  
  SPIClass& _SPI_BUS;
  uint8_t   _CS;
  
  // Read Delay
  const unsigned int _read_delay = 30;

  // Commands for AMT20
  const uint8_t _cmd_idle = 0x00;
  const uint8_t _cmd_pos = 0x10;
  const uint8_t _cmd_zeroSet = 0x70;
  const uint8_t _cmd_zeroed = 0x80;
  const uint8_t _cmd_noData = 0xA5;

  // Functions
  uint8_t send_command(uint8_t command);

  // Variables
  uint16_t _encoder_raw;
  float    _encoder_angle;
  uint8_t  _encoder_data[2];
  

};

