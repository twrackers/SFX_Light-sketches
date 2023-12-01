#include <OneShot.h>
#include <FadeLED_Func.h>

#include "Glint.h"

// Use symmetric rise and fall curves.
#define TIME_ON 250L
#define TIME_OFF TIME_ON

// Constructor
Glint::Glint(Adafruit_TLC5947* device, const uint16_t pin) : 
  OneShot(TIME_ON, false),
  m_fader(new FadeLED_Func(device, pin, TIME_ON, TIME_OFF)),
  m_magn(0.0) {
  }

// Update method
// @return `true` if fader object updated, `false` otherwise
bool Glint::update() {
  
  bool updated = false;

  // Time to update fader object?
  if (m_fader->update()) {
    // Get normalized fader output.
    double val = m_fader->get();
    // Set adjusted value.
    m_fader->set(val * val * m_magn);
    updated = true;
  }
  
  // Time to update parent object?
  if (OneShot::update()) {
    // Write `true` to fader if on rising half of flash,
    // `false` otherwise.
    m_fader->write(OneShot::isTriggered());
  }
  
  return updated;
  
}

// Trigger method
// @param magn normalized peak brightness of flash (1.0 = full on)
void Glint::trigger(const double magn) {

  // If fader is off (between flashes), set peak magnitude
  // and trigger one-shot parent object.
  if (m_fader->isOff()) {
    m_magn = min(magn, 1.0);
    OneShot::trigger();
  }
  
}
