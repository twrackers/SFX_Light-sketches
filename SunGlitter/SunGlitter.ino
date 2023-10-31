#include <StateMachine.h>

#include <Adafruit_TLC5947.h>
#include <SPI.h>

#if defined(LOG)
#include <Streaming.h>
#endif

#include "Glint.h"

//#define DIM(X) (sizeof(X) / sizeof((X)[0]))

#define NUM_TLC5947 1

#define DATA_PIN 4
#define CLOCK_PIN 5
#define LATCH_PIN 6
#define OE_PIN -1

// Maximum for NUM_PWM is (24 * NUM_TLC5947)
#define NUM_PWM 12

Adafruit_TLC5947 tlc(NUM_TLC5947, CLOCK_PIN, DATA_PIN, LATCH_PIN);

Glint* pwm_chans[NUM_PWM];

void setup() {

#if defined(LOG)
  Serial.begin(115200);
  while (!Serial) {}
#endif
  
  randomSeed(analogRead(A0));

  for (byte pin = 0; pin < NUM_PWM; ++pin) {
    pwm_chans[pin] = new Glint(&tlc, pin);
  }

  tlc.begin();
  tlc.write();
  
  if (OE_PIN >= 0) {
    pinMode(OE_PIN, OUTPUT);
    digitalWrite(OE_PIN, LOW);
  }

  pinMode(LED_BUILTIN, OUTPUT);
#if defined(LOG)
  digitalWrite(LED_BUILTIN, HIGH);
#else
  digitalWrite(LED_BUILTIN, LOW);
#endif
  
}

StateMachine pacer(10, true);

#define ALGO_1

#define PERIOD 1000

uint32_t prev = 0L;
uint32_t now;
#if defined(LOG)
int count = 1000;
#endif

void loop() {

#if defined(ALGO_1)

  if (random(PERIOD) == 0L) {
    pwm_chans[random(NUM_PWM)]->trigger();
#if defined(LOG)
    if (count) {
      now = millis();
      if (prev) {
        Serial.println(now - prev);
      }
      prev = now;
      --count;
    } else {
      digitalWrite(LED_BUILTIN, LOW);
    }
#endif
  }
  
#elif defined(ALGO_2)

  int which = random(PERIOD * NUM_PWM);
  if (which < NUM_PWM) {
    pwm_chans[which]->trigger();
#if defined(LOG)
    if (count) {
      now = millis();
      if (prev) {
        Serial.println(now - prev);
      }
      prev = now;
      --count;
    } else {
      digitalWrite(LED_BUILTIN, LOW);
    }
#endif
  }
  
#else
#error Define ALGO_1 or ALGO_2
#endif
  
  StateMachine::updateAll(pwm_chans, NUM_PWM);

  if (pacer.update()) {
    tlc.write();
  }
  
}
