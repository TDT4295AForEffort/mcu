#include "em_gpio.h"
#include "em_cmu.h"
#include "game.h"
#include "utils.h"

#define DEBOUNCE_TICKS 20

uint32_t prev_counter;

void GPIO_EVEN_IRQHandler(void)
{
  GPIO_IntClear(0xFFFF);
  if (counter - prev_counter < DEBOUNCE_TICKS) return;
  prev_counter = counter;
  destroy_block();
#ifdef DEVBOARD
  GPIO_PinOutToggle(gpioPortE, 2);
#else
  GPIO_PinOutToggle(gpioPortF, 5);
#endif
}

void GPIO_ODD_IRQHandler(void)
{
  GPIO_IntClear(0xFFFF);
  if (counter - prev_counter < DEBOUNCE_TICKS) return;
  prev_counter = counter;
  place_block();
#ifdef DEVBOARD
  GPIO_PinOutToggle(gpioPortE, 3);
#else
  GPIO_PinOutToggle(gpioPortE, 9);
  //test
#endif
}

void initGPIO(void)
{
  prev_counter = 0;
  CMU_ClockEnable(cmuClock_GPIO, true);
  NVIC_EnableIRQ(GPIO_EVEN_IRQn);
  NVIC_EnableIRQ(GPIO_ODD_IRQn);
#ifdef DEVBOARD
  GPIO_PinModeSet(gpioPortB, 9, gpioModeInputPullFilter, 1);
  GPIO_PinModeSet(gpioPortB, 10, gpioModeInputPullFilter, 1);

  GPIO_ExtIntConfig(gpioPortB, 9, 9, 0, 1, true);
  GPIO_ExtIntConfig(gpioPortB, 10, 10, 0, 1, true);

  GPIO_PinModeSet(gpioPortE, 2, gpioModePushPull, 0);
  GPIO_PinModeSet(gpioPortE, 3, gpioModePushPull, 0);


#else
  GPIO_PinModeSet(gpioPortF, 5, gpioModePushPull, 1); //Init LED pin PCB
  GPIO_PinOutSet(gpioPortF, 5);
  GPIO_PinModeSet(gpioPortE, 9, gpioModePushPull, 1); //Init LED pin PCB
  GPIO_PinOutSet(gpioPortE, 9);
  GPIO_PinModeSet(gpioPortD, 10, gpioModePushPull, 1); //Init LED pin PCB
  GPIO_PinOutSet(gpioPortD, 10);

  GPIO_PinModeSet(gpioPortD, 1, gpioModeInputPullFilter, 1); // Init button pcb
  GPIO_PinModeSet(gpioPortA, 8, gpioModeInputPullFilter, 1); // Init button pcb

  GPIO_ExtIntConfig(gpioPortD, 1, 1, 0, 1, true);
  GPIO_ExtIntConfig(gpioPortA, 8, 8, 0, 1, true);
#endif
}
