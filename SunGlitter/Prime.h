#ifndef _PRIME__H_
#define _PRIME__H_

#include <math.h>
#include <stdint.h>

// Calculate largest integer less than or
// equal to square root of value.
// @param y integer value for which to estimate square root
// @return integer equivalent to floor(sqrt(y))
uint32_t isqrt(const uint32_t y) {

  // Square roots of 0 or 1 are 0 and 1 respectivey.
  if (y < 2) {
    return y;
  }

  // Recursively estimate lower limit of sqrt(y).
  uint32_t sm = isqrt(y >> 2) << 1;
  // Create upper limit.
  uint32_t lg = sm + 1;
  // Binary search between lower and upper limits.
  return ((lg * lg) > y) ? sm : lg;
  
}

// Determine if value is prime.
// @param y integer value to be tested
// @return true if y is prime, false otherise
bool is_prime(const uint32_t y) {
  
  if (y == 2) {
    // 2 is prime.
    return true;
  } else if (y < 2 || (y % 2) == 0) {
    // 0, 1, and all even numbers > 2 are NOT prime.
    return false;
  } else {
    // Only need to test odd values from 3 up to sqrt(y)
    uint32_t m = isqrt(y);
    for (uint32_t t = 3; t <= m; t += 2) {
      // Not prime if t divides evenly into y.
      if (y % t == 0) return false;
    }
    // No divisors, must be prime.
    return true;
  }
  
}

// Find a random prime number less than given value.
// Random number generator is seeded the first time
// `find_random_prime` is called.
// @param mx maximum value (exclusive) for returned value
// @return randomly selected prime number in range [0,mx)
uint32_t find_random_prime(const uint32_t mx) {
  
  static bool seeded = false;

  // Only seed random number generator the first time.
  if (!seeded) {
    randomSeed(analogRead(A0));
    seeded = true;
  }

  // Test random values unti one that's prime is found.
  uint32_t t;
  do {
    t = random(mx);
  } while (!is_prime(t));
  return t;
  
}

#endif
