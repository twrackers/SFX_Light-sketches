#ifndef _PRIME__H_
#define _PRIME__H_

#include <math.h>
#include <stdint.h>

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
    uint32_t m = (uint32_t) sqrt((double) val);
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
