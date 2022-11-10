#include "random.h"

// A single iteration of Bob Jenkins' One-At-A-Time hashing algorithm.
uint32_t hash(uint32_t* x) {
  *x += (*x << 10);
  *x ^= (*x >> 6);
  *x += (*x << 3);
  *x ^= (*x >> 11);
  *x += (*x << 15);
  return *x;
}

// Construct a float with half-open range [0:1] using low 23 bits.
// All zeroes yields 0.0, all ones yields the next smallest representable value below 1.0.
float floatConstruct(uint32_t m) {
  const uint32_t ieeeMantissa = 0x007FFFFF; // binary32 mantissa bitmask
  const uint32_t ieeeOne = 0x3F800000; // 1.0 in IEEE binary32

  m &= ieeeMantissa;                     // Keep only mantissa bits (fractional part)
  m |= ieeeOne;                          // Add fractional part to 1.0

    //int to float
  float  f = *(float*)(&m);       // Range [1:2]
  return f - 1.0;                        // Range [0:1]
}

float randomFloat() {
  return floatConstruct(hash(&seed));
}

void srandom(uint32_t new_seed) {
  seed = new_seed;
}

