#include "em_gpio.h"
#include "em_cmu.h"
#include "game.h"

void GPIO_EVEN_IRQHandler(void)
{
  GPIO_IntClear(0x5555);
  GPIO_PinOutToggle(gpioPortE, 2);
  place_block();
}

void GPIO_ODD_IRQHandler(void)
{
  GPIO_IntClear(0xAAAA);
  GPIO_PinOutToggle(gpioPortE, 3);
  destroy_block();
}

void initGPIO(void)
{
  CMU_ClockEnable(cmuClock_GPIO, true);

  GPIO_PinModeSet(gpioPortF, 5, gpioModePushPull, 1); //Init LED pin PCB
  GPIO_PinOutSet(gpioPortF, 5);

  GPIO_PinModeSet(gpioPortB, 9, gpioModeInputPullFilter, 1);
  GPIO_PinModeSet(gpioPortB, 10, gpioModeInputPullFilter, 1);

  GPIO_PinModeSet(gpioPortE, 2, gpioModePushPull, 0);
  GPIO_PinModeSet(gpioPortE, 3, gpioModePushPull, 0);

  NVIC_EnableIRQ(GPIO_EVEN_IRQn);
  NVIC_EnableIRQ(GPIO_ODD_IRQn);

  // Falling-edge interrupts
  GPIO_ExtIntConfig(gpioPortB, 9, 9, 0, 1, true);
  GPIO_ExtIntConfig(gpioPortB, 10, 10, 0, 1, true);
}
