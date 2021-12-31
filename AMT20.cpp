#include "AMT20.h"

SPISettings SPI_write(AMT20_BUS_SPEED, MSBFIRST, SPI_MODE0);

AMT20::AMT20(SPIClass& AMT20_SPI_BUS, uint8_t AMT20_CS) :
  _SPI_BUS(AMT20_SPI_BUS),
  _CS(AMT20_CS)
{
  _encoder_raw = 0;
  _encoder_angle = 0;
}

void AMT20::init()
{
  // set up the chip select pin
  pinMode(_CS, OUTPUT);
  digitalWrite(_CS, HIGH);
  delay(200);
  
  return;
}

bool AMT20::read()
{
  // Read the encoder.  Returns 1 on success, 0 on failure
  
  uint8_t ret, msb, lsb, bail;
  ret = 0;
  bail = 0;

  // Send read position command
  send_command(_cmd_pos);

  // Response is _cmd_pos when data is ready
  while (ret != _cmd_pos) {
    ret = send_command(_cmd_idle);
    bail++;

    if (bail >= 10) 
    {
      // Didn't get the response in time
	  return (0);
    }
  }

  msb = send_command(_cmd_idle);
  lsb = send_command(_cmd_idle);
  
  // Combine bytes to get final value
  _encoder_raw = (uint16_t)msb << 8 | (uint16_t)lsb; 

  // All done  
  return (1);
}

uint16_t AMT20::raw_value()
{
    return (_encoder_raw);
}

float AMT20::angle()
{
    _encoder_angle = _encoder_raw * 360.0f / 4096.0f;
    return (_encoder_angle);
}

bool AMT20::zero()
{

  // Zeros the AMT20.  Return 1 on success, 0 on failure
	
  uint8_t ret, bail;
  ret = 0;
  bail = 0;

  send_command(_cmd_zeroSet);

  while (ret != _cmd_zeroed) {
    ret = send_command(_cmd_idle);
    bail++;

    if (bail > 50)
    { 
      // Set failed
      return (0);
    }
  }
  
  // All done
  return (1);
  
}

uint8_t AMT20::send_command(uint8_t command)
{
  uint8_t retval;

  // CS needs to be asserted after SPI beginTransaction otherwise funny things with the clock happen...
  _SPI_BUS.beginTransaction(SPI_write);
  digitalWrite(_CS, LOW);
  retval = _SPI_BUS.transfer(command);
  digitalWrite(_CS, HIGH);
  _SPI_BUS.endTransaction();
  
  // Need to slow the bus down between reads
  delayMicroseconds(_read_delay);

  return (retval);
}
