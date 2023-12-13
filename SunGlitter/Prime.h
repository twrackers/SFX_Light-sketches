#ifndef _PRIME__H_
#define _PRIME__H_

#include <math.h>
#include <stdint.h>

uint32_t isqrt(const uint32_t y) {

  if (y < 2) {
    return y;
  }

  uint32_t sm = isqrt(y >> 2) << 1;
  uint32_t lg = sm + 1;
  if ((lg * lg) > y) {
    return sm;
  } else {
    return lg;
  }
  
}

bool is_prime(const uint32_t val) {
  
  static bool seeded = false;
  
  if (!seeded) {
    randomSeed(analogRead(A0));
    seeded = true;
  }
  
  if (val == 2) {
    return true;
  } else if (val < 2 || (val % 2) == 0) {
    return false;
  } else {
    uint32_t m = isqrt(val);
    for (uint32_t t = 3; t <= m; t += 2) {
      if (val % t == 0) return false;
    }
    return true;
  }
  
}

uint32_t find_random_prime(const uint32_t mx) {
  uint32_t t;
  do {
    t = random(mx);
  } while (!is_prime(t));
  return t;
}

#endif
