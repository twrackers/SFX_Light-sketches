#ifndef _NIGHT_STROBE__H_
#define _NIGHT_STROBE__H_

#include <OneShot.h>
#include <FadeLED_Lin.h>

#include "GPIOs.h"

// Night turn-on time (msec)
#define NIGHT_RAMP_UP 300
// Night turn-off time (msec)
#define NIGHT_RAMP_DOWN 500
// Night full-on time (msec)
#define NIGHT_FULL_ON(p) (p - NIGHT_RAMP_UP - NIGHT_RAMP_DOWN - 100)

class NightStrobe : public OneShot {

  private:
    FadeLED_Lin* m_led;

  public:

    NightStrobe(const uint32_t period) : 
      OneShot(NIGHT_RAMP_UP + NIGHT_FULL_ON(period), false) {
      m_led = new FadeLED_Lin(NIGHT_STROBE,
                              NIGHT_RAMP_UP, NIGHT_RAMP_DOWN,
                              ACTIVE_LOW);
    }

    bool update() {
      if (OneShot::update()) {
        m_led->write(OneShot::isTriggered());
      }
      m_led->update();
    }
  
};

#endif
