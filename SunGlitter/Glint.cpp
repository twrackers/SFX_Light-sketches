#include "Glint.h"

// Use symmetric rise and fall curves.
#define TIME_ON 250L
#define TIME_OFF TIME_ON

static Adafruit_TLC5947* sm_device = NULL;

void Glint::begin(Adafruit_TLC5947* device) {
  sm_device = device;
}

// Constructor
Glint::Glint(const byte chan) : 
  OneShot(TIME_ON, false),
  m_fader(new FadeLED_Func(NO_PWM, TIME_ON, TIME_OFF)),
  m_magn(0.0),
  m_chan(chan) {
  }

// Update method
// @return `true` if fader object updated, `false` otherwise
bool Glint::update() {
  
  bool updated = false;

  // Time to update fader object?
  if (m_fader->update()) {
    // Get normalized fader output.
    double val = m_fader->get();
    // Write adjusted value to TLC device.
    sm_device->setPWM(m_chan, (uint16_t) (val * val * m_magn));
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

  // TLC5947 is 12-bit PWM driver, so set scale to match.
  const double scale = (double) 0x0FFF;
  
  // If fader is off (between flashes), set peak magnitude
  // and trigger one-shot parent object.
  if (m_fader->isOff()) {
    m_magn = min(magn, 1.0) * scale;
    OneShot::trigger();
  }
  
}
