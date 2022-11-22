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
#include "sl_simple_button_instances.h"
#include "sl_simple_button_btn0_config.h"
#include "sl_simple_button_config.h"

uint8_t transmitBuffer[BUFFERSIZE];
uint8_t receiveBuffer[BUFFERSIZE];

GameBlock game_map[GAME_MAP_SIZE][GAME_MAP_SIZE];
Player player;
Enemy enemies[NUM_ENEMIES];

sl_button_state_t previous_state = 0;

uint32_t sample_x, sample_y, sample_view;
int c;

void app_init(void) {
  CHIP_Init();
  /* Enabling clock to USART 1 and 2*/
  CMU_ClockEnable(cmuClock_USART1, true);
  CMU_ClockEnable(cmuClock_USART2, true);

  initADC();
//  initGPIO();

  sl_button_init(&sl_button_btn0);

  srandom(sampleJoystick(adcSingleInputCh7));
  init_map();
  init_player();
  init_enemies();

    CMU_ClockEnable(cmuClock_GPIO, true);

    GPIO_PinModeSet(gpioPortF, 5, gpioModePushPull, 1); //Init LED pin PCB
    GPIO_PinOutSet(gpioPortF, 5);

    //GPIO_PinModeSet(gpioPortB, 9, gpioModeInputPullFilter, 1);
    //GPIO_PinModeSet(gpioPortB, 10, gpioModeInputPullFilter, 1);

    GPIO_PinModeSet(gpioPortB, 12, gpioModeInputPullFilter, 1); // Init button pcb
    GPIO_PinModeSet(gpioPortA, 14, gpioModeInputPullFilter, 1); // Init button pcb

//    GPIO_PinModeSet(gpioPortD, 2, gpioModeInputPullFilter, 1); // Init button devboard

    GPIO_PinModeSet(gpioPortE, 2, gpioModePushPull, 0);
    GPIO_PinModeSet(gpioPortE, 3, gpioModePushPull, 0);

//    NVIC_EnableIRQ(GPIO_EVEN_IRQn);
//    NVIC_EnableIRQ(GPIO_ODD_IRQn);

    // Falling-edge interrupts
    //GPIO_ExtIntConfig(gpioPortB, 9, 9, 0, 1, true);
    //GPIO_ExtIntConfig(gpioPortB, 10, 10, 0, 1, true);

    // Devboard falling edge interrupt
//    GPIO_ExtIntConfig(gpioPortD, 2, 2, 0, 1, true);

    // Pcb falling edge interrupt
    GPIO_ExtIntConfig(gpioPortB, 12, 12, 0, 1, true);
    GPIO_ExtIntConfig(gpioPortA, 14, 14, 0, 1, true);

  //SPI_setup(USART2_NUM, GPIO_POS0, true); //PCB MCU to pins out
  //SPI_setup(USART1_NUM, GPIO_POS1, true); //PCB MCU straight to FPGA
  SPI_setup(USART1_NUM, GPIO_POS0, true);
  /* Setting up RX interrupt for master */
  //SPI2_setupRXInt(NO_RX, NO_RX);
  SPI1_setupRXInt(NO_RX, NO_RX);
//  setupSWOForPrint();
  c = 0;
}

/***************************************************************************/
/**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void) {
  c++;
  // Sample joystick in X-direction
  /*sample_x = sampleJoystick(adcSingleInputCh5);

  // Sample joystick in Y-direction
  sample_y = sampleJoystick(adcSingleInputCh4);

  // Sample joystick rotate
  sample_view = sampleJoystick(adcSingleInputCh6);*/

   //sample joystick for pcb
   sample_x = sampleJoystick(adcSingleInputCh0);
   sample_y = sampleJoystick(adcSingleInputCh3);
   sample_view = sampleJoystick(adcSingleInputCh4);

   // Button
   sl_button_poll_step(&sl_button_btn0);
   sl_button_state_t state = sl_button_get_state(&sl_button_btn0);
   if (previous_state == 0 && state == 1) {
       GPIO_PinOutToggle(gpioPortE, 2);
       GPIO_PinOutToggle(gpioPortF, 5); //Toggle LED pcb
       place_block();
//       ITM_SendChar('1');
   } else {
//       ITM_SendChar('0');
   }
   previous_state = state;


  // printConvertedJoystickSample(sample_x);
  //ITM_SendChar('A');
  //ITM_SendChar('\n');
  // printConvertedJoystickSample(sample_y);
  // ITM_SendChar(' ');
  const float dt = 0.01;
  move_player(convertSample(sample_x), convertSample(sample_y), dt);
  turn_player(convertSample(sample_view), dt);

  // move_player(0.0, 1.0, dt);
  // turn_player(convertSample(sample_y), dt);
  move_enemies(dt);

  // Fill transmit buffer with updated values of the game state
  populate_spi_transmit_buffer(0, (uint16_t)BUFFERSIZE, player, game_map,
                               enemies, transmitBuffer);
  // For master to send
  //USART2_sendBuffer(transmitBuffer, BUFFERSIZE); //Debug
  USART1_sendBuffer(transmitBuffer, BUFFERSIZE); //Regular
  // ITM_SendChar('\n');
  // memset(receiveBuffer, '\0', BUFFERSIZE);
  // Application process.
  if (c > 100) {

    //char buf[150];
    /*print_str("dir x: ");
    gcvt(player.x_dir, 6, buf);
    print_str(buf);
    ITM_SendChar(' ');
    print_str("dir y: ");
    gcvt(player.y_dir, 6, buf);
    print_str(buf);
    ITM_SendChar('\n');

    printConvertedJoystickSample(sample_view);
    ITM_SendChar('\n');
    print_str("x pos: ");
    gcvt(player.x_pos, 6, buf);
    print_str(buf);
    ITM_SendChar(' ');
    print_str("fixed point as int: ");
    snprintf(buf, 10, "%ld ", float_to_fixed(player.x_pos));
    print_str(buf);
    ITM_SendChar('\n');


    print_str("y pos: ");
    gcvt(player.y_pos, 6, buf);
             print_str(buf);
             ITM_SendChar(' ');
             print_str("fixed point as int: ");
             snprintf(buf, 10, "%ld ", float_to_fixed(player.y_pos));
             print_str(buf);
             ITM_SendChar('\n');*/
    //print_gamestate();
    //print_str(transmitBuffer);
    //ITM_SendChar('\n');
    // print_str("size of ")
    // printJoystickSample(sample_x);
    // ITM_SendChar(' ');
    /*print_str("Joystick sample y: ");
    printJoystickSample(sample_y);
    ITM_SendChar('\n');*/
    // player.y_pos = 1.0;
    // c = 0;
    /*for(int i = 532; i < 564; i++){ //20, 52 to get bitmap map
        snprintf(buf, 6, "%02x ", transmitBuffer[i]);
        print_str(buf);
        ITM_SendChar('\n');
    }

    print_gamestate();*/
    c = 0;
  }
}
