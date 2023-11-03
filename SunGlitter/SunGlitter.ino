#include <StateMachine.h>

#include <Adafruit_TLC5947.h>
#include <SPI.h>

#define USE_TRINKET

#if defined(USE_TRINKET)

#include <avr/power.h>

#undef LOG

#endif

#include "Glint.h"

#define NUM_TLC5947 1

#if defined(USE_TRINKET)

#define DATA_PIN 2
#define CLOCK_PIN 1
#define LATCH_PIN 0

#else

#define DATA_PIN 4
#define CLOCK_PIN 5
#define LATCH_PIN 6

#endif

#define OE_PIN -1

// Maximum for NUM_PWM is (24 * NUM_TLC5947)
#define NUM_PWM 12

// Driver for PWM interface
Adafruit_TLC5947 tlc(NUM_TLC5947, CLOCK_PIN, DATA_PIN, LATCH_PIN);

// Pointers to Glint objects (1 per LED)
Glint* pwm_chans[NUM_PWM];

void setup() {

#if defined(USE_TRINKET)
  if (F_CPU == 16000000L) {
    clock_prescale_set(clock_div_1);
  }
#endif

#if defined(LOG)
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

  // If OE pin is being used, set it up.
  // (In this application, it won't be used.)
  if (OE_PIN >= 0) {
    pinMode(OE_PIN, OUTPUT);
    digitalWrite(OE_PIN, LOW);
  }

  // Set up LED_BUILTIN (pin 13).
  // If logging is enabled, the LED will be on while data is written
  // to the serial connection.
  pinMode(LED_BUILTIN, OUTPUT);
#if defined(LOG)
  digitalWrite(LED_BUILTIN, HIGH);
#else
  digitalWrite(LED_BUILTIN, LOW);
#endif
  
}

// Outputs will be written to PWM driver every 10 msec.
StateMachine pacer(10, true);

// Select switching algorithm, for testing.
#define ALGO_2

// Average time betwen triggers (msec)
#define PERIOD 1000

#if defined(LOG)

void write_log(const int which) {

  static uint32_t s_prev = 0L;
  static uint32_t s_now = 0L;
  static int s_count = 1000;
  
  if (s_count) {
    s_now = millis();
    if (s_prev) {
      Serial.print(which);
      Serial.print(" ");
      Serial.println(s_now - s_prev);
    }
    s_prev = s_now;
    --s_count;
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }
  
}

#endif

void loop() {

#if defined(ALGO_1)

  // At random, trigger lighting cycle for one PWM channel.
  // This algorithm calls random() once or twice.
  
  if (random(PERIOD) == 0L) {
    int which = random(NUM_PWM);
    pwm_chans[which]->trigger();
#if defined(LOG)
    write_log(which);
#endif
  }
  
#elif defined(ALGO_2)

  // At random, trigger lighting cycle for one PWM channel.
  // This algorithm calls random() once always.
  
  int which = random(PERIOD * NUM_PWM);
  if (which < NUM_PWM) {
    pwm_chans[which]->trigger();
#if defined(LOG)
    write_log(which);
#endif
  }
  
#else
#error Define ALGO_1 or ALGO_2
#endif

  // Update output values for all PWM chanels.
  StateMachine::updateAll(pwm_chans, NUM_PWM);

  // Periodically update the PWM driver with output values.
  if (pacer.update()) {
    tlc.write();
  }
  
}
