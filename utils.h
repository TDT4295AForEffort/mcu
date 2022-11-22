#pragma once
#include <stdint.h>

//comment out when on pcb
//#define DEVBOARD

void setupSWOForPrint(void);
void print_str(char str[]);
void print_gamestate();
float randomFloat();
void srandom(uint32_t new_seed);

extern uint32_t counter;

extern uint32_t seed;
