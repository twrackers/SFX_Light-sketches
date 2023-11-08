#include <StateMachine.h>
#include <Pulse.h>

#include <Adafruit_TLC5947.h>

#include "Glint.h"

#define NUM_TLC5947 1

// GPIO pins are defined for Adafruit Itsy Bitsy 32u4.
#define DATA_PIN 5
#define CLOCK_PIN 7
#define LATCH_PIN 9

// Maximum for NUM_PWM is (24 * NUM_TLC5947)
#define NUM_PWM 12

// Driver for PWM interface
Adafruit_TLC5947 tlc(NUM_TLC5947, CLOCK_PIN, DATA_PIN, LATCH_PIN);

// Pointers to Glint objects (1 per LED)
Glint* pwm_chans[NUM_PWM];

void setup() {

#ifdef LOG
  Serial.begin(115200);
  while (!Serial) {}
#endif

  // Seed the RNG.
  randomSeed(analogRead(A0));

  // Create Glint object for each LED connected to a PWM channel.
  for (byte pin = 0; pin < NUM_PWM; ++pin) {
    pwm_chans[pin] = new Glint(&tlc, pin);
  }

  // Initialize TLC5947 device.
  tlc.begin();
  tlc.write();
  
}

// Outputs will be written to PWM driver every 10 msec.
#define REAL_TIME true
StateMachine pacer(10, REAL_TIME);

// Built-in LED will flash when a PWM is triggered.
Pulse activ(LED_BUILTIN, 3);

// Average time between PWM triggers (msec)
#define PERIOD 500

void loop() {

  // At random, trigger lighting cycle for one PWM channel.
  int which = random(PERIOD * NUM_PWM);
  uint32_t magn = random(15, 101);
  if (which < NUM_PWM) {
    pwm_chans[which]->trigger((double) (magn * magn) / 10000.0);
    activ.trigger();
  }
  
  // Update output values for all PWM chanels.
  StateMachine::updateAll(pwm_chans, NUM_PWM);

  // Periodically update the PWM driver with output values.
  // Driver cannot keep up with being updated on every pass
  // through loop().
  if (pacer.update()) {
    tlc.write();
  }

  // Update LED_BUILTIN flasher.
  activ.update();  
  
}
