#ifndef _GPIOS__H_
#define _GPIOS__H_

// ---------- Configuration ----------

// Invert (active-low) flag, uncomment one line
//#define ACTIVE_LOW true
#define ACTIVE_LOW false

// Board selection, uncomment one line
//#define ARDUINO_UNO
#define ADAFRUIT_GEMMA_V2

// ---------- Definitions ----------

#if ACTIVE_LOW == true
  #define LIGHT_ON LOW
  #define LIGHT_OFF HIGH
#else
  #define LIGHT_ON HIGH
  #define LIGHT_OFF LOW
#endif

#if defined(ARDUINO_UNO)

  // Mode select pin (input), LOW = night, HIGH = day
  #define SELECT 8
  // Day strobe (white LED, output)
  #define DAY_STROBE 9
  // Night strobe (red LED, PWM output)
  #define NIGHT_STROBE 10
  
#elif defined(ADAFRUIT_GEMMA_V2)

  // Mode select pin (digital input), LOW = night, HIGH = day
  #define SELECT 2
  // Day strobe (white LED, output)
  #define DAY_STROBE 1
  // Night strobe (red LED, PWM output)
  #define NIGHT_STROBE 0
  
#else

  #error Must define target board
  
#endif

// ---------- Functions ----------

void gpio_setup() {

  pinMode(SELECT, INPUT_PULLUP);
  
  pinMode(DAY_STROBE, OUTPUT);
  digitalWrite(DAY_STROBE, LIGHT_OFF);
  
  pinMode(NIGHT_STROBE, OUTPUT);
  digitalWrite(NIGHT_STROBE, LIGHT_OFF);

}

bool is_daytime() {

  return (digitalRead(SELECT) == HIGH);
  
}

#endif
