#include <OneShot.h>
#include <FadeLED_Func.h>

#include "Glint.h"

#define TIME_ON 250L
#define TIME_OFF 250L

Glint::Glint(Adafruit_TLC5947* device, const uint16_t pin) : 
  OneShot(TIME_ON, false),
  m_glint(new FadeLED_Func(device, pin, TIME_ON, TIME_OFF)),
  m_pin(pin) {}
      
bool Glint::update() {
  
  bool updated = false;

  if (m_glint->update()) {
    double val = m_glint->get();
    m_glint->set(val * val);
    updated = true;
  }
  
  if (OneShot::update()) {
    m_glint->write(OneShot::isTriggered());
  }
  
  return updated;
  
}

void Glint::trigger() {
  
  if (m_glint->isOff()) {
    OneShot::trigger();
  }
  
}
