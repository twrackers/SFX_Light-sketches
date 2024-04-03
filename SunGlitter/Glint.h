#ifndef _GLINT__H_
#define _GLINT__H_

#include <Adafruit_TLC5947.h>

#include <OneShot.h>

#include <FadeLED_Func.h>

// `Glint` object IS A `OneShot` (inherits methods).
// `Glint` object HAS A `FadeLED_Func` (does not inherit).
class Glint : public OneShot {

  private:
    FadeLED_Func* m_fader;      // (pointer to) LED fader
    double m_magn;              // flash magnitude during cycle
    byte m_chan;                // PWM channel (0 to 23)

  public:
    // Static method
    static void begin(Adafruit_TLC5947* device);
    
    // Constructor
    Glint(const byte chan);
    
    // Overrides of inherited methods
    virtual bool update();
    void trigger(const double magn);
  
};

#endif
