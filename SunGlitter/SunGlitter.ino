#include <StateMachine.h>
#include <Pulse.h>

#include <Adafruit_TLC5947.h>

#include "Glint.h"

// GPIO pins are defined for Adafruit Itsy Bitsy 32u4.
#define DATA_PIN 5
#define CLOCK_PIN 7
#define LATCH_PIN 9

// Number of 24-channel PWM drivers daisy-chained
#define NUM_TLC5947 1

// Maximum for NUM_PWM is (24 * `NUM_TLC5947`)
#define NUM_PWM 24

// Driver for PWM interface
Adafruit_TLC5947 tlc(NUM_TLC5947, CLOCK_PIN, DATA_PIN, LATCH_PIN);

// Pointers to `Glint` objects (1 per LED)
Glint* pwm_chans[NUM_PWM];

void setup() {

  // Seed the RNG.
  randomSeed(analogRead(A0));

  // Create `Glint` object for each LED connected to a PWM channel.
  for (byte pin = 0; pin < NUM_PWM; ++pin) {
    pwm_chans[pin] = new Glint(&tlc, pin);
  }

  // Initialize TLC5947 device.
  tlc.begin();
  tlc.write();
  
}

// Average time between PWM triggers (msec)
#define PERIOD 750

#define MMIN 500
#define MMAX 1000

#define REAL_TIME true
#define NON_REAL_TIME false

// Triggers to PWM cycles
StateMachine pwm_pacer(1, REAL_TIME);

// Outputs will be written to PWM driver every 25 msec.
StateMachine out_pacer(25, REAL_TIME);

// Built-in LED will flash when a PWM is triggered.
Pulse activ(LED_BUILTIN, 3);

void loop() {
  
  if (pwm_pacer.update()) {
    // Try to choose a random PWM channel.
    int which = random(PERIOD * NUM_PWM);
    // If an existing PWM channel is chosen...
    if (which < NUM_PWM) {
      // ... trigger lighting cycle for that channel.
      // Amplitude of PWM flash will be random, from about 6%
      // to 100% full-on.
      uint32_t magn = random(MMIN, MMAX + 1);
      pwm_chans[which]->trigger(pow((double) magn / (double) MMAX, 4.0));
      // Trigger a flash of the built-in LED too.
      activ.trigger();
    }
  }
  
  // Update output values for all PWM chanels.
  StateMachine::updateAll(pwm_chans, NUM_PWM);

  // Periodically update the PWM driver with output values.
  // Driver cannot keep up with being updated on every pass
  // through loop().
  if (out_pacer.update()) {
    tlc.write();
  }

  // Update LED_BUILTIN flasher.
  activ.update();  
  
}
