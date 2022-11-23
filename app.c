/***************************************************************************/
/**
 * @file
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/
#include "app.h"

#include "adc.h"
#include "em_adc.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_device.h"
#include "em_emu.h"
#include "em_gpio.h"
#include "game.h"
#include "gpio.h"
#include "serialize.h"
#include "spi.h"
#include "usart.h"
#include "utils.h"
#include "gpio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint8_t transmitBuffer[BUFFERSIZE];
uint8_t receiveBuffer[BUFFERSIZE];

GameBlock game_map[GAME_MAP_SIZE][GAME_MAP_SIZE];
Player player;
Enemy enemies[NUM_ENEMIES];

uint32_t sample_x, sample_y, sample_view;
uint32_t counter;

void app_init(void) {
  counter = 0;

  CHIP_Init();
  /* Enabling clock to USART 1 and 2*/
  CMU_ClockEnable(cmuClock_USART1, true);
  CMU_ClockEnable(cmuClock_USART2, true);

  initADC();
  initGPIO();

  srandom(sampleJoystick(adcSingleInputCh7));
  init_map();
  init_player();
  init_enemies();

#ifdef DEVBOARD
  SPI_setup(USART1_NUM, GPIO_POS1, true);
#else
  SPI_setup(USART1_NUM, GPIO_POS0, true); //PCB MCU straight to FPGA
#endif
  /* Setting up RX interrupt for master */
  SPI1_setupRXInt(NO_RX, NO_RX);
  //setupSWOForPrint(); //only needed when debugging on a devboard
}

/***************************************************************************/
/**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void) {
  counter++;

#ifdef DEVBOARD
  sample_x = sampleJoystick(adcSingleInputCh5);
  sample_y = sampleJoystick(adcSingleInputCh4);
  sample_view = sampleJoystick(adcSingleInputCh6);
#else
  sample_x = sampleJoystick(adcSingleInputCh0);
  sample_y = sampleJoystick(adcSingleInputCh3);
  sample_view = sampleJoystick(adcSingleInputCh4);
#endif

  const float dt = 0.01;
  move_player(-convertSample(sample_x), -convertSample(sample_y), dt);
  turn_player(convertSample(sample_view), dt);

  move_enemies(dt);

  // Fill transmit buffer with updated values of the game state
  populate_spi_transmit_buffer(0, (uint16_t)BUFFERSIZE, player, game_map,
                               enemies, transmitBuffer);
  USART1_sendBuffer(transmitBuffer, BUFFERSIZE); //Regular
}
