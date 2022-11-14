#include <stdio.h>
#include "em_adc.h"
#include "em_gpio.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_device.h"
#include "em_emu.h"
#include "adc.h"

void initADC() {
  CMU_ClockEnable(cmuClock_ADC0, true);
  ADC_Init_TypeDef init = ADC_INIT_DEFAULT;
  ADC_Init(ADC0, &init);
}

void waitForAdcConversion() {
  while(!(ADC0->STATUS & _ADC_STATUS_SINGLEDV_MASK));
}

uint32_t sampleJoystick(ADC_SingleInput_TypeDef channel) {
  ADC_InitSingle_TypeDef initSingle = ADC_INITSINGLE_DEFAULT;
  initSingle.reference  = adcRefVDD;
  initSingle.input = channel;
  ADC_InitSingle(ADC0, &initSingle);

  ADC_Start(ADC0, adcStartSingle);
  waitForAdcConversion();
  return ADC_DataSingleGet(ADC0);
}

void printJoystickSample(uint32_t sample) {
  char buf[150];
  snprintf(buf, sizeof buf, "%ld", (uint32_t) sample);
  for (unsigned int i = 0; i < sizeof(uint32_t); i++) {
    ITM_SendChar(buf[i]);
  }
}

void printConvertedJoystickSample(uint32_t sample) {
  char buf[150];
  gcvt(convertSample(sample), 6, buf);
  for (unsigned int i = 0; i < sizeof(uint32_t); i++) {
    ITM_SendChar(buf[i]);
  }
}

float convertSample(uint32_t sample) {
  if (sample > 1500 && sample < 2500) {
      return 0.0;
  }
  float fSample = sample;
  return (fSample / 2048) - 1.0;

}

