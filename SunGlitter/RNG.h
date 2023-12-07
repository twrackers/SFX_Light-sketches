#ifndef _RNG__H_
#define _RNG__H_

#include <stdint.h>

// Random Number Generator
// Returns random values in half-open range [0, (2^31)-1),
// Works on processor (Arduino) that doesn't support 64-bit integers.

class RNG {

  private:
    uint32_t m_state;    // internal state value

  public:
    // Constructor
    RNG();

    // Set state to seed value
    void seed(const uint32_t x);

    // Return random value from full range [0, (2^31)-1)
    // @return random value
    uint32_t random();

    // Return random value from range [0, mx)
    // @param mx maximum value (exclusive) of returned range
    // @return random value
    uint32_t random(const uint32_t mx);

    // Return random value from range [mn, mx)
    // @param mn minimum value (inclusive) of returned range
    // @param mx maximum value (exclusive) of returned range
    // @return random value
    uint32_t random(const uint32_t mn, const uint32_t mx);
  
};

#endif
