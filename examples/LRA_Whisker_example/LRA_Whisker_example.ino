#include <Wire.h>
#include "Adafruit_DRV2605.h"

Adafruit_DRV2605 drv;

void setup() {
  SerialUSB.begin(9600);
  SerialUSB.println("DRV test");
  drv.begin();

  drv.selectLibrary(1);

  // I2C trigger by sending 'go' command
  // default, internal trigger when sending GO command
  drv.setMode(DRV2605_MODE_INTTRIG);
}

uint8_t effect = 1;

void loop() {
  SerialUSB.print("Effect #"); SerialUSB.println(effect);
  Wire.beginTransmission(0x70);
  Wire.write(0x04 + 0);  //0x04 + port (0-3)
  Wire.endTransmission();

  // set the effect to play
  drv.setWaveform(0, effect);  // play effect
  drv.setWaveform(1, 0);       // end waveform

  // play the effect!
  drv.go();

  // wait a bit
  delay(500);

  effect++;
  if (effect > 117) effect = 1;
}
