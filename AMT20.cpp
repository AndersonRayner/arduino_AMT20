#include "AMT20.h"

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

    delay(2);
    
    ret = send_command(_cmd_idle);
    bail++;

    if (bail > 20)
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

  // Need to slow the bus down between reads
  delayMicroseconds(_read_delay);

  // CS needs to be asserted after SPI beginTransaction otherwise funny things with the clock happen...
  // We also need to delay a little after asserting the CS for faster MCUs that are too quick for the AMT
  _SPI_BUS.beginTransaction(SPISettings(_bus_speed, MSBFIRST, SPI_MODE0));
  digitalWrite(_CS, LOW);
  delayMicroseconds(1);
  retval = _SPI_BUS.transfer(command);
  delayMicroseconds(1);
  digitalWrite(_CS, HIGH);
  _SPI_BUS.endTransaction();
  
  // All done
  return (retval);
}
