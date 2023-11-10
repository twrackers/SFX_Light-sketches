#include "OneLight.h"

// Class (static) data members
    
Adafruit_TLC59711* OneLight::sm_tlc;   // ptr to device driver
uint32_t OneLight::sm_period;          // period of full cycle [msec]
uint32_t OneLight::sm_ramp;            // ramp up/down times [msec]
uint32_t OneLight::sm_start;           // start time [msec]

// Class (static) method

void OneLight::begin(Adafruit_TLC59711* tlc,
                     const byte lights, 
                     const uint32_t period, const uint32_t waitFor) {
  sm_tlc = tlc;
  sm_period = period;
  sm_ramp = sm_period / lights;
  sm_start = millis() + waitFor;
}

// Object methods

// Constructor
OneLight::OneLight(const byte chan) :
m_oneshot(new OneShot(sm_ramp, false)),
m_fader(new FadeLED_Func(sm_tlc, chan, sm_ramp, sm_ramp)) {
  m_next = chan * sm_ramp + sm_start;
}

// Update method
// @return `true` if it was time to start new light cycle
bool OneLight::update() {
  
  bool updated = false;

  // If next time has been reached...
  if (millis() >= m_next) {
    // ... advance next time by one period interval...
    m_next += sm_period;
    // ... and trigger the one-shot.
    m_oneshot->trigger();
    updated = true;
  }
  m_oneshot->update();
  
  // Based on state of one-shot, update fader.
  double f = m_fader->get();
  m_fader->set(pow(f, 1.5));
  m_fader->write(m_oneshot->isTriggered());
  m_fader->update();

  return updated;
  
}
