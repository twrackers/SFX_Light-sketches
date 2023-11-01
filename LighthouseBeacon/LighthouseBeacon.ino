#include <FadeLED_Func.h>
#include <OneShot.h>

#define USE_12CH

#if defined(USE_12CH)
#include <Adafruit_TLC59711.h>
#elif defined(USE_24CH)
#include <Adafruit_TLC5947.h>
#else
#error Define either USE_12CH or USE_24CH
#endif
#include <SPI.h>

#if LOG
#include <Streaming.h>
#endif

// This example code drives an array of LEDs simulating a lighthouse.
//
// Dependencies:
//    https://github.com/twrackers/FadeLED-library
//    https://github.com/twrackers/OneShot-library
//    https://github.com/twrackers/StateMachine-library

#if defined(USE_12CH)

#define NUM_TLC59711 1

#define data 11
#define clock 12

Adafruit_TLC59711 tlc(NUM_TLC59711, clock, data);

#elif defined(USE_24CH)

#define NUM_TLC5947 1

#define data 4
#define clock 5
#define latch 6
#define oe -1

Adafruit_TLC5947 tlc(NUM_TLC5947, clock, data, latch);

#endif

#define NUM_LIGHTS 12
// PERIOD in milliseconds
#define PERIOD 4800L
#define RAMP_UP (PERIOD / NUM_LIGHTS)
#define RAMP_DN (RAMP_UP)

StateMachine pacer(10, true);

#if defined(LOG)
StateMachine logger(50, true);
#endif

// Ticker object's `update()` returns `true` once every `m_period`
// milliseconds, starting when `millis()` reaches `start`.
class Ticker {

  private:
    const uint32_t m_period;  // milliseconds between `true` updates
    uint32_t m_next;          // time of upcoming `true` update

  public:
    Ticker(const uint32_t period, const uint32_t start) :
    m_period(period), m_next(start) { }

    bool update() {
      // If next time has been reached...
      if (millis() >= m_next) {
        // ... advance next time by one period interval...
        m_next += m_period;
        // ... and return `true`.
        return true;
      }
      // Otherwise, return `false`.
      return false;
    }
    
};

// `OneShot` objects trigger ramp-up of faders.
OneShot* oneshots[NUM_LIGHTS];
// Faders ramp up/down output of PWM channels.
FadeLED_Func* faders[NUM_LIGHTS];
// Tickers trigger oneshots.
Ticker* tickers[NUM_LIGHTS];

void setup()
{

#if defined(LOG)
  Serial.begin(115200);
  while (!Serial) {
  }
#endif

  // Create objects for PWM channels.
  for (byte pin = 0; pin < NUM_LIGHTS; ++pin) {
    faders[pin] = new FadeLED_Func(&tlc, pin, RAMP_UP, RAMP_DN);
    oneshots[pin] = new OneShot(RAMP_UP, false);
    tickers[pin] = new Ticker(PERIOD, (1000 + pin * RAMP_UP));
  }

  // Initialize the TLC device.
  tlc.begin();
  tlc.write();

#if defined(oe)
  // If /OE pin exists on device, pull it low to enable outputs.
  if (oe >= 0) {
    pinMode(oe, OUTPUT);
    digitalWrite(oe, LOW);
  }
#endif

}

void loop()
{

  // Update all the `OneShot` objects.
  StateMachine::updateAll(oneshots, NUM_LIGHTS);
  
  // Set the desired output values.
#if defined(LOG)
  logger.update();
#endif
  if (pacer.update()) {
    
    for (int i = 0; i < NUM_LIGHTS; ++i) {
      // `trig` will be `true` on rising fade,
      // `false otherwise.
      bool trig = oneshots[i]->isTriggered();
      // Create non-linear fade.
      FadeLED_Func* fader = faders[i];
      double f = fader->get();
      fader->set(pow(f, 1.5));
      // Set rising or falling fade, depending on `trig`.
      fader->write(trig);
      // Update the fader's state.
      fader->update();
      // If ticker updates, trigger matching `OneShot`.
      if (tickers[i]->update()) {
        oneshots[i]->trigger();
      }
    }
    // Update all channels on PWM driver.
    tlc.write();
    
  }

}
