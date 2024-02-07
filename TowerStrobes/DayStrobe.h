#ifndef _DAY_STROBE__H_
#define _DAY_STROBE__H_

#include <Pulse.h>

#include "GPIOs.h"

// Day strobe on time (msec)
#define DAY_ON 10

class DayStrobe : public Pulse {

  public:
  
    DayStrobe() : Pulse(DAY_STROBE, LIGHT_ON, DAY_ON) {
    }
    
};

#endif
