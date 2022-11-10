#ifndef RANDOM_H_
#define RANDOM_H_

#include <stdint.h>

uint32_t hash(uint32_t* x);

float floatConstruct(uint32_t m);

float randomFloat();

void srandom(uint32_t new_seed);

extern uint32_t seed;

#endif
