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

//#define DIM(X) (sizeof(X) / sizeof((X)[0]))

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
#define PERIOD 4800L  // milliseconds
#define RAMP_UP (PERIOD / NUM_LIGHTS)
#define RAMP_DN (RAMP_UP)

StateMachine pacer(10, true);
#if defined(LOG)
StateMachine logger(50, true);
#endif

class Ticker {

  private:
    const uint32_t m_period;
    uint32_t m_next;

  public:
    Ticker(const uint32_t period, const uint32_t start) :
    m_period(period), m_next(start) { }

    bool update() {
      if (millis() >= m_next) {
        m_next += m_period;
        return true;
      }
      return false;
    }
    
};

OneShot* oneshots[NUM_LIGHTS];
FadeLED_Func* faders[NUM_LIGHTS];
Ticker* tickers[NUM_LIGHTS];

void setup()
{

#if defined(LOG)
  Serial.begin(115200);
  while (!Serial) {
  }
#endif

  for (byte pin = 0; pin < NUM_LIGHTS; ++pin) {
    faders[pin] = new FadeLED_Func(&tlc, pin, RAMP_UP, RAMP_DN);
    oneshots[pin] = new OneShot(RAMP_UP, false);
    tickers[pin] = new Ticker(PERIOD, (1000 + pin * RAMP_UP));
  }
  
  tlc.begin();
  tlc.write();

#if defined(oe)
  if (oe >= 0) {
    pinMode(oe, OUTPUT);
    digitalWrite(oe, LOW);
  }
#endif

}

void loop()
{

  StateMachine::updateAll(oneshots, NUM_LIGHTS);
  
  // Set the desired output values.
#if defined(LOG)
  logger.update();
#endif
  if (pacer.update()) {
    for (int i = 0; i < NUM_LIGHTS; ++i) {
      bool trig = oneshots[i]->isTriggered();
      FadeLED_Func* fader = faders[i];
      double f = fader->get();
      fader->set(pow(f, 1.5));
      fader->write(trig);
      fader->update();
      if (tickers[i]->update()) {
        oneshots[i]->trigger();
      }
    }
    tlc.write();
  }

}
