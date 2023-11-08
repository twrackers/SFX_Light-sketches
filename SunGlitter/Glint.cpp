#include <OneShot.h>
#include <FadeLED_Func.h>

#include "Glint.h"

#define TIME_ON 250L
#define TIME_OFF 250L

Glint::Glint(Adafruit_TLC5947* device, const uint16_t pin) : 
  OneShot(TIME_ON, false),
  m_fader(new FadeLED_Func(device, pin, TIME_ON, TIME_OFF)),
  m_pin(pin), m_magn(0.0) {}
      
bool Glint::update() {
  
  bool updated = false;

  if (m_fader->update()) {
    double val = m_fader->get();
    m_fader->set(val * val * m_magn);
    updated = true;
  }
  
  if (OneShot::update()) {
    m_fader->write(OneShot::isTriggered());
  }
  
  return updated;
  
}

void Glint::trigger(const double magn) {

  if (m_fader->isOff()) {
    m_magn = min(magn, 1.0);
#ifdef LOG
    Serial.println(m_magn * 100.0);
#endif
    OneShot::trigger();
  }
  
}
