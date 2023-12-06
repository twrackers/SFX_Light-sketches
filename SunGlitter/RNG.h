#ifndef _RNG__H_
#define _RNG__H_

#include <stdint.h>

class RNG {

  private:
    uint32_t m_state;

  public:
    RNG();
    void seed(const uint32_t x);
    uint32_t random();
    uint32_t random(const uint32_t mx);
    uint32_t random(const uint32_t mn, const uint32_t mx);
  
};

#endif
