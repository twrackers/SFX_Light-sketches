#include "OneLight.h"

// This example code drives an array of LEDs simulating a lighthouse.
//
// Dependencies:
//    https://github.com/twrackers/FadeLED-library
//    https://github.com/twrackers/OneShot-library
//    https://github.com/twrackers/StateMachine-library

// Number of 12-channel PWM drivers daisy-chained
#define NUM_TLC59711 1

// Maximum for NUM_PWM is (12 * `NUM_TLC59711`)
#define NUM_LIGHTS 12

// NOTE: Adafruit_TLC59711 library required a patch to the constructor
// which defaults to the built-in SPI interface.  The library
// originally set up the SPI interface to use SPI_MODE0, but spurious
// flickering occurred on channels that were supposed to be at zero
// output.  Changing the mode to SPI_MODE3 appears to have fixed that.

// Driver for PWM interface, using hardware SPI
Adafruit_TLC59711 tlc(NUM_TLC59711);

// Pointers to `OneLight` objects (1 per LED)
OneLight* lights[NUM_LIGHTS];

// Full rotation period [msec]
#define PERIOD 4800L
// Startup delay after begin() [msec]
#define WAIT_FOR 1000L

void setup()
{

  // Assign class constants.
  OneLight::begin(&tlc, NUM_LIGHTS, PERIOD, WAIT_FOR);

  // Create `OneLight` object for each PWM channel.
  for (byte chan = 0; chan < NUM_LIGHTS; ++chan) {
    lights[chan] = new OneLight(chan);
  }

  // Initialize the TLC device.
  tlc.begin();
  tlc.simpleSetBrightness(127);   // full brightness
  tlc.write();

}

// Outputs will be written to PWM driver every 10 msec.
#define REAL_TIME true
StateMachine pacer(10, REAL_TIME);

void loop()
{

  // Time to update the PWM outputs?
  if (pacer.update()) {
    
    // Update each `OneLight` object.
    for (byte chan = 0; chan < NUM_LIGHTS; ++chan) {
      lights[chan]->update();
    }
    
    // Update all channels on PWM driver.
    tlc.write();
    
  }

}
