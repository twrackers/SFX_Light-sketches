#ifndef _ONE_LIGHT__H_
#define _ONE_LIGHT__H_

#include <Adafruit_TLC59711.h>
#include <SPI.h>

#include <OneShot.h>
#include <FadeLED_Func.h>

// `OneLight` object's `update()` returns `true` once every
// `sm_period` milliseconds, starting when `millis()` reaches `m_next`.
class OneLight {

  private:
    // Class data members
    static Adafruit_TLC59711* sm_tlc;   // ptr to device driver
    static uint32_t sm_period;          // period of full cycle [msec]
    static uint32_t sm_ramp;            // ramp up/down times [msec]
    static uint32_t sm_start;           // start time [msec]

    // Object data members
    OneShot* m_oneshot;     // this HAS A `OneShot`
    FadeLED_Func* m_fader;  // this HAS A `FadeLD_Func`
    uint32_t m_next;        // time of upcoming `true` update

  public:
    // Class method
    //
    // Called before `OneLight` objects are created to set values
    // shared by all objects.
    //
    // @param tlc pointer to driver object
    // @param lights number of PWM channels to be used
    // @param period time to perform one full rotation [msec]
    // @param waitFor delay after this call before rotations begin [msec]
    static void begin(Adafruit_TLC59711* tlc,
                      const byte lights,
                      const uint32_t period, const uint32_t waitFor);

    // Object methods

    // Constructor
    // @param chan PWM channel
    OneLight(const byte chan);

    // Update states of member objects
    // @return `true` if member `OneShot` updated
    bool update();
    
};


#endif
