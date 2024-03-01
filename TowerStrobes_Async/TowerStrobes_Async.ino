#include <Arduino.h>

#include <StateMachine.h>

#include "DayStrobe.h"
#include "NightStrobe.h"

// Strobe periods (msec)
#define PERIOD_DAY 1500
#define PERIOD_NIGHT 1600
#define CHANGE_INTERVAL 1500

// Day and night strobes can have different rates.
StateMachine pacer_day(PERIOD_DAY, true);
StateMachine pacer_night(PERIOD_NIGHT, true);

// The actual strobe objects.
DayStrobe day_strobe;
NightStrobe night_strobe(PERIOD_NIGHT);

// Updated when changing between day and night,
// to pause both strobes briefly.
uint32_t change_time = 0L;

void setup() {
  gpio_setup();
}

// Current time of day
enum E_TOD {
  EDawn, EDay, EDusk, ENight
} state = ENight;

void loop() {
  // Is it time up trigger either strobe?
  bool up_day = pacer_day.update();
  bool up_night = pacer_night.update();

  // Get status of day/night input.
  bool daytime_now = is_daytime();
  
  uint32_t now = millis();

  // Take action depending on current state.
  
  if (state == EDawn) {
    
    // Has dawn timed out?
    if ((long) (now - change_time) >= 0) {
      // If so, it's day now.
      state = EDay;
      // Trigger day strobe if it's time to.
      if (up_day) {
        day_strobe.trigger();
      }
    }
    
  } else if (state == EDay) {
    
    // Has day/night input changed?
    if (!daytime_now) {
      // If so, it's dusk now, set timeout and don't trigger
      // either strobe.
      state = EDusk;
      change_time = now + CHANGE_INTERVAL;
    } else {
      // Otherwise, trigger day strobe if it's time to.
      if (up_day) {
        day_strobe.trigger();
      }
    }
    
  } else if (state == EDusk) {
    
    // Has dusk timed out?
    if ((long) (now - change_time) >= 0) {
      // If so, it's night now.
      state = ENight;
      // Trigger night strobe if it's time to.
      if (up_night) {
        night_strobe.trigger();
      }
    }
    
  } else /* if (state == ENight) */ {
    
    // Has day/night input changed?
    if (daytime_now) {
      // If so, it's dawn now, set timeout and don't trigger
      // either strobe.
      state = EDawn;
      change_time = now + CHANGE_INTERVAL;
    } else {
      // Otherwise, trigger night strobe if it's time to.
      if (up_night) {
        night_strobe.trigger();
      }
    }
    
  }
  
  day_strobe.update();
  night_strobe.update();
}
