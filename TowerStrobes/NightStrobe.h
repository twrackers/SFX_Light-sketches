#ifndef _NIGHT_STROBE__H_
#define _NIGHT_STROBE__H_

#include <OneShot.h>
#include <FadeLED_Lin.h>

#include "GPIOs.h"

// Night turn-on time (msec)
#define NIGHT_RAMP_UP 300
// Night full-on time (msec)
#define NIGHT_FULL_ON 600
// Night turn-off time (msec)
#define NIGHT_RAMP_DOWN 500

class NightStrobe : public OneShot {

  private:
    FadeLED_Lin* m_led;

  public:

    NightStrobe() : OneShot(NIGHT_RAMP_UP + NIGHT_FULL_ON, false) {
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

    void trigger() {
      OneShot::trigger();
    }
  
};

#endif
