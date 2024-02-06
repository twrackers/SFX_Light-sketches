#include <Arduino.h>

#include <StateMachine.h>

#include "DayStrobe.h"
#include "NightStrobe.h"

// Strobe period (msec)
#define PERIOD 1500

StateMachine pacer(PERIOD, true);

DayStrobe day_strobe;
NightStrobe night_strobe;

void setup() {
  gpio_setup();
}

void loop() {
  if (pacer.update()) {
    if (is_daytime()) {
      day_strobe.trigger();
    } else {
      night_strobe.trigger();
    }
  }

  day_strobe.update();
  night_strobe.update();
}
