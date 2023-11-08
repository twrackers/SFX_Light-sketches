#include "OneLight.h"

// This example code drives an array of LEDs simulating a lighthouse.
//
// Dependencies:
//    https://github.com/twrackers/FadeLED-library
//    https://github.com/twrackers/OneShot-library
//    https://github.com/twrackers/StateMachine-library

#define NUM_TLC59711 1

// NOTE: Adafruit_TLC5971 library required a patch to the constructor
// which defaults to the built-in SPI interface.  The library originally
// set up the SPI interface to use SPI_MODE0, but spurious flickering
// occurred on channels that were supposed to be at zero output.  Changing
// the mode to SPI_MODE3 appears to have fixed that.

Adafruit_TLC59711 tlc(NUM_TLC59711);

#define NUM_LIGHTS 12

// Full rotation period [msec]
#define PERIOD 4800L
// Startup delay after begin() [msec]
#define WAIT_FOR 1000L

StateMachine pacer(10, true);

// Tickers trigger PWM in sequence.
OneLight* lights[NUM_LIGHTS];

void setup()
{

  OneLight::begin(&tlc, NUM_LIGHTS, PERIOD, WAIT_FOR);

  // Create objects for PWM channels.
  for (byte chan = 0; chan < NUM_LIGHTS; ++chan) {
    lights[chan] = new OneLight(chan);
  }

  // Initialize the TLC device.
  tlc.begin();
  tlc.simpleSetBrightness(127);
  tlc.write();

}

void loop()
{

  // Set the desired output values.
  if (pacer.update()) {
    
    for (byte chan = 0; chan < NUM_LIGHTS; ++chan) {
      lights[chan]->update();
    }
    
    // Update all channels on PWM driver.
    tlc.write();
    
  }

}
