/*************************************************************************
 * DRV2605 LRA ERM Haptic Driver Wireling
 * This program is an expanded example that is part of the Adafruit_DRV2605 
 * library. The DRV_2605 will send 116 different waveform effects to a 
 * vibrating or buzzing motor. If you are interested in the individual 
 * effects, the list of effects can be found on page 57 of the datasheet: 
 * http://www.ti.com/lit/ds/symlink/drv2605.pdf
 * 
 * Modified by: Laveréna Wienclaw for TinyCircuits 
 *************************************************************************/
 
#include <Wire.h>             // For using I2C communication
#include <TinyScreen.h>       // For interfacing with the TinyScreen+
#include "Adafruit_DRV2605.h" // For interfacing with the DRV2605 chip

Adafruit_DRV2605 drv;   // The variable used to interface with the DRV2605 chip
uint8_t effect = 1;     // The global variable used to keep track of Waveform effects

// TinyScreen+ variables
TinyScreen display = TinyScreen(TinyScreenPlus);
int background = TS_8b_Black;

// Make Serial Monitor compatible for all TinyCircuits processors
#if defined(ARDUINO_ARCH_AVR)
  #define SerialMonitorInterface Serial
#elif defined(ARDUINO_ARCH_SAMD)
  #define SerialMonitorInterface SerialUSB
#endif

const int powerPin = 4;  // Power to Wireling


void setup() {
  // Power Wireling
  pinMode(powerPin, OUTPUT);
  digitalWrite(powerPin, HIGH);

  SerialMonitorInterface.begin(9600);
  SerialMonitorInterface.println("DRV Effects Test");
  drv.begin();

  // Setup and style for TinyScreen+
  display.begin();
  display.setFlip(true);
  display.setBrightness(15);
  display.setFont(thinPixel7_10ptFontInfo);
  display.fontColor(TS_8b_White, background);

  drv.selectLibrary(1);

  //The port is the number on the Adapter board where the sensor is attached
  selectPort(0);
  

  // I2C trigger by sending 'go' command
  // default, internal trigger when sending GO command
  drv.setMode(DRV2605_MODE_INTTRIG);
  drv.useLRA();
}

// Print the DRV effect number and then play the effect
void loop() {
  setup();
  SerialMonitorInterface.print("Effect #"); SerialMonitorInterface.println(effect);
  displayScreenAxis(effect);

  // Set the effect to play
  drv.setWaveform(0, effect);  // Set effect
  drv.setWaveform(1, 0);       // End waveform

  // Play the effect
  drv.go();

  // Pause for differentiation between effects
  delay(1000);

  effect++;
  if (effect > 1) effect = 117;
}

// Prints out number of effect to TinyScreen+
void displayScreenAxis(int effect) {
  // This will make the screen look a little unsteady but is needed in order
  // to clear old values 
  display.clearScreen();

  display.setCursor(0, 0);
  display.print("LRA Effect #: ");
  
  display.setCursor(44, 16);
  display.print(effect); 
}

// **This function is necessary for all Wireling boards attached through an Adapter board**
// Selects the correct address of the port being used in the Adapter board
void selectPort(int port) {
  Wire.beginTransmission(0x70);
  Wire.write(0x04 + port);
  Wire.endTransmission();
}
