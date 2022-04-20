// Software for running the AMT20 rotary encoder

#include <SPI.h>

#include <AMT20.h>

#define CS_ENCODER 10

AMT20 amt20(SPI,CS_ENCODER);

void setup() {

  Serial.begin(115200);
  Serial.print("Starting AMT203-V Demo Program\n");

  // start encoder
  SPI.begin();
  amt20.init();

  // zero encoder
  if (amt20.zero())
  {
    Serial.print("  AMT203-V successfully zeroed\n");
  }
  else
  {
    Serial.print("  AMT203-V zero'ing wasn't successful...\n");
  }

  delay(50);

}


void loop() {

  if (amt20.read())
  {
    //Serial.print(amt20.raw_value()); Serial.print("\n");
    Serial.print(amt20.angle()    ); Serial.print("\n");
  } 
  else
  {
    Serial.print("  AMT203-V read failed!\n");
  }
  
  // Slow the program down a bit for readability, this sensor is fast
  delay(100);
  
}