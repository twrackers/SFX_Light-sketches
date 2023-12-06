#include "RNG.h"

uint32_t lcg_parkmiller(uint32_t* state) {
  
  // Precomputed parameters for Schrage's method
  const uint32_t M = 0x7fffffffL;
  const uint32_t A = 48271L;
  const uint32_t Q = M / A;    // 44488
  const uint32_t R = M % A;    //  3399

  uint32_t div = *state / Q;  // max: M / Q = A = 48,271
  uint32_t rem = *state % Q;  // max: Q - 1     = 44,487

  int32_t s = rem * A;  // max: 44,487 * 48,271 = 2,147,431,977 = 0x7fff3629
  int32_t t = div * R;  // max: 48,271 *  3,399 =   164,073,129
  int32_t result = s - t;

  if (result < 0)
    result += M;

  return *state = result;
  
}

RNG::RNG() {
}

void RNG::seed(const uint32_t x) {

  m_state = x;
  
}

uint32_t RNG::random() {

  return lcg_parkmiller(&m_state);
  
}

uint32_t RNG::random(const uint32_t mx) {

  return random() % mx;
}

uint32_t RNG::random(const uint32_t mn, const uint32_t mx) {

  return random() % (mx - mn) + mn;
  
}
