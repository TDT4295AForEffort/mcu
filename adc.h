#pragma once
#include "em_adc.h"
#include <stdint.h>

void initADC();
uint32_t sampleJoystick(ADC_SingleInput_TypeDef channel);
void printJoystickSample(uint32_t sample);
void printConvertedJoystickSample(uint32_t sample);
float convertSample(uint32_t sample);
