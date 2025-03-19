#ifndef _NIGHT_STROBE__H_
#define _NIGHT_STROBE__H_

#include <OneShot.h>
#include <FadeLED_Lin.h>

#include "GPIOs.h"

// Night turn-on time (msec)
#define NIGHT_RAMP_UP_TIME 300
// Night full-on time (msec)
#define NIGHT_FULL_ON_TIME 600
// Night turn-off time (msec)
#define NIGHT_RAMP_DOWN_TIME 500

class NightStrobe : public OneShot {

  private:
    FadeLED_Lin* m_led;

  public:

    NightStrobe() : OneShot(NIGHT_RAMP_UP_TIME + NIGHT_FULL_ON_TIME, false) {
      m_led = new FadeLED_Lin(NIGHT_STROBE,
                              NIGHT_RAMP_UP_TIME, NIGHT_RAMP_DOWN_TIME,
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
