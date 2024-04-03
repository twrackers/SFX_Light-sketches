#include <StateMachine.h>
#include <Pulse.h>

#include "Prime.h"
#include "Glint.h"
#include "RNG.h"

// GPIO pins are defined for Adafruit Metro Mini.
// Adafruit TLC5947 doesn't appear to be compatible with Arduino
// hardware SPI, so bit-banging library must be used instead.
#define DATA_PIN 4
#define CLOCK_PIN 5
#define LATCH_PIN 6

// Number of 24-channel PWM drivers daisy-chained
#define NUM_TLC5947 1

// Maximum for NUM_PWM is (24 * `NUM_TLC5947`)
#define NUM_PWM 24

// Driver for PWM interface
Adafruit_TLC5947 tlc(NUM_TLC5947, CLOCK_PIN, DATA_PIN, LATCH_PIN);

// Pointers to `Glint` objects (1 per LED)
Glint* pwm_chans[NUM_PWM];

// Random-number-generator objects
// Each RNG object has its own state, so calls to either does not affect the
// sequence generated by the other.
RNG rand_pwm;      // to select which PWM channel to trigger
RNG rand_magn;     // to select a random brightness of a flash

void setup() {

  // Seed the RNGs.
  rand_pwm.seed(find_random_prime(0x10000));
  rand_magn.seed(find_random_prime(0x10000));

  // Initialize TLC5947 device.
  pinMode(LED_BUILTIN, OUTPUT);
  if (!tlc.begin()) {
    digitalWrite(LED_BUILTIN, HIGH);
    while (true) delay(10);
  }
  tlc.write();

  // Associate `Glint` class with TLC5947 device.
  Glint::begin(&tlc);

  // Create `Glint` object for each LED connected to a PWM channel.
  for (byte pin = 0; pin < NUM_PWM; ++pin) {
    pwm_chans[pin] = new Glint(pin);
  }
  
}

// Average time between PWM triggers (msec)
#define PERIOD 750

// Range of random magnitudes of flashes [MMIN, MMAX)
// in per mille (parts per thousand)
#define MMIN 500
#define MMAX 1000

// Symbols for booleans
#define REAL_TIME true
#define NON_REAL_TIME false

// Triggers to PWM cycles
StateMachine pwm_pacer(1, REAL_TIME);

// Outputs will be written to PWM driver every 25 msec.
StateMachine out_pacer(25, REAL_TIME);

// Built-in LED will flash when a PWM is triggered.
// A `Pulse` object has a `OneShot` member object
// which is retriggerable.
Pulse activ(LED_BUILTIN, 5);

void loop() {

  const uint32_t RAND_PWM_RANGE = PERIOD * NUM_PWM;
  
  if (pwm_pacer.update()) {
    // Try to choose a random PWM channel.
    int which = rand_pwm.random(RAND_PWM_RANGE);
    // If an existing PWM channel is chosen...
    if (which < NUM_PWM) {
      // ... trigger lighting cycle for that channel.
      // Amplitude of PWM flash will be random, from about 3%
      // to 100% full-on.
      uint32_t magn = rand_magn.random(MMIN, MMAX + 1);
      pwm_chans[which]->trigger(pow((double) magn / (double) MMAX, 5.0));
      // Trigger a flash of the built-in LED too.
      activ.trigger();
    }
  }

  // Update LED_BUILTIN flasher.
  activ.update();
  
  // Update output values for all PWM chanels.
  StateMachine::updateAll(pwm_chans, NUM_PWM);

  // Periodically update the PWM driver with output values.
  // Driver cannot keep up with being updated on every pass
  // through loop().
  if (out_pacer.update()) {
    tlc.write();
  }

}
