#ifndef _GLINT__H_
#define _GLINT__H_

#include <Adafruit_TLC5947.h>

#include <OneShot.h>
#include <FadeLED_Func.h>

// `Glint` object IS A `OneShot` (inherits methods).
// `Glint` object HAS A `FadeLED_Func` (does not inherit).
class Glint : public OneShot {

  private:
    FadeLED_Func* m_fader;  // (pointer to) LED fader
    const uint16_t m_pin;   // PWM channel on TLC5947
    double m_magn;          // flash magnitude during cycle

  public:
    // Constructor
    Glint(Adafruit_TLC5947* device, const uint16_t pin);
    // Overrides of inherited methods
    virtual bool update();
    void trigger(const double magn);
  
};

#endif
