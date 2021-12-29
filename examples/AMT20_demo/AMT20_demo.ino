// Software for running the AMT20 rotary encoder
//
// Currently only tested on Arduino Pro Mini
//
// Highest known working Arduino versions
// Arduino  : 1.6.9
// AVR core : 1.6.16

#include <SPI.h>

#include <AMT20.h>

#define CS_ENCODER 10

AMT20 amt20(SPI,CS_ENCODER);

void setup() {

  Serial.begin(115200);
  Serial.print("Starting airspeed card\n");

  // start encoder
  SPI.begin();
  amt20.init();

  // zero encoder
  amt20.zero();

}


void loop() {
  amt20.read();
  
  //Serial.print(amt20.raw_value()); Serial.print("\n");
  Serial.print(amt20.angle()    ); Serial.print("\n");

  delay(10);
}
