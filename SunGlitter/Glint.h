#ifndef _GLINT__H_
#define _GLINT__H_

#include <Adafruit_TLC5947.h>

#include <OneShot.h>
#include <FadeLED_Func.h>

class Glint : public OneShot {

  private:
    FadeLED_Func* m_fader;
    const uint16_t m_pin;

  public:
    Glint(Adafruit_TLC5947* device, const uint16_t pin);
      
    virtual bool update();

    void trigger();
  
};

#endif
