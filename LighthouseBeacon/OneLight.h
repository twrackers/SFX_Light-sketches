#ifndef _ONE_LIGHT__H_
#define _ONE_LIGHT__H_

#include <Adafruit_TLC59711.h>
#include <SPI.h>

#include <OneShot.h>
#include <FadeLED_Func.h>

// OneLight object's `update()` returns `true` once every `sm_period`
// milliseconds, starting when `millis()` reaches `m_next`.
class OneLight {

  private:
    // Class data members
    static Adafruit_TLC59711* sm_tlc;   // ptr to device driver
    static uint32_t sm_period;          // period of full cycle [msec]
    static uint32_t sm_ramp;            // ramp up/down times [msec]
    static uint32_t sm_start;           // start time [msec]

    // Object data members
    OneShot* m_oneshot;
    FadeLED_Func* m_fader;
    uint32_t m_next;        // time of upcoming `true` update

  public:
    // Class method
    static void begin(Adafruit_TLC59711* tlc,
                      const byte lights,
                      const uint32_t period, const uint32_t waitFor);

    // Object methods
    OneLight(const byte chan);
    bool update();
    
};


#endif
