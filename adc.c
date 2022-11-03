#include "em_adc.h"
#include "em_gpio.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_device.h"
#include "em_emu.h"

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

float sampleToInterval(uint32_t sample) {
   if (sample > 2000 || sample < 2300) {
       return 0;
   }

  return (((float)sample - 0) / (4095 - 0)) * (1 - (-1)) + (-1);
}
