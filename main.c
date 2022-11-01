/***************************************************************************//**
 * @file main.c
 * @brief main() function.
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
#include "sl_component_catalog.h"
#include "sl_system_init.h"
#if defined(SL_CATALOG_POWER_MANAGER_PRESENT)
#include "sl_power_manager.h"
#endif
#if defined(SL_CATALOG_KERNEL_PRESENT)
#include "sl_system_kernel.h"
#else // SL_CATALOG_KERNEL_PRESENT
#include "sl_system_process_action.h"
#endif // SL_CATALOG_KERNEL_PRESENT

#include <string.h>
#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "spi.h"
#include "usart.h"
#include "game.h"
#include "serialize.h"
#include "stdint.h"
#include <stdlib.h>
//#include "time.h"

#define BUFFERSIZE 8210 //Size of header+player+map
#define DELTA_TIME 0.001 //To be used to calculate player position and direction

uint8_t transmitBuffer[BUFFERSIZE];
uint8_t receiveBuffer[BUFFERSIZE];
Player player;
//TestStruct *transmit_test;

GameBlock game_map[GAME_MAP_SIZE][GAME_MAP_SIZE];

void init(void) {
  /* Enabling clock to USART 1 and 2*/
    CMU_ClockEnable(cmuClock_USART1, true);
    CMU_ClockEnable(cmuClock_USART2, true);
    CMU_ClockEnable(cmuClock_GPIO, true);

    //Init player
    init_player();
    //player = malloc(20);
    /*player.x_pos = 4.0;
    player.y_pos = 4.3;
    player.vision_angle = 2.7;
    player.x_dir = 3.8;
    player.y_dir = 6.8;*/

    /*transmit_test = malloc(BUFFERSIZE);
    transmit_test->one = 1;
    transmit_test->two = 1;
    transmit_test->three = 1;
    transmit_test->four = 1;
    transmit_test->five = 1;*/

    init_map(); //Init the map
    populate_spi_transmit_buffer(0, (uint8_t) 8210/*Buffer size*/, player, game_map, transmitBuffer);

    //populate_spi_transmit_buffer_test(transmit_test, transmitBuffer);
    //populate_spi_transmit_buffer_test_player(player, transmitBuffer);

    //master setup
    /* Setup USART */
    SPI_setup(USART1_NUM, GPIO_POS1, true);

    /* Setting up RX interrupt for master */
    SPI1_setupRXInt(NO_RX, NO_RX);

    //slave setup
    /*SPI_setup(USART1_NUM, GPIO_POS1, false);
    SPI1_setupSlaveInt(receiveBuffer, BUFFERSIZE, NO_TX, NO_TX);
    memset(receiveBuffer, '\0', BUFFERSIZE);*/
}

void setupSWOForPrint(void)
{
  /* Enable GPIO clock. */
  CMU->HFPERCLKEN0 |= CMU_HFPERCLKEN0_GPIO;

  /* Enable Serial wire output pin */
  GPIO->ROUTE |= GPIO_ROUTE_SWOPEN;

  #if defined(_EFM32_GIANT_FAMILY) || defined(_EFM32_LEOPARD_FAMILY) || defined(_EFM32_WONDER_FAMILY)
  /* Set location 0 */
  GPIO->ROUTE = (GPIO->ROUTE & ~(_GPIO_ROUTE_SWLOCATION_MASK)) | GPIO_ROUTE_SWLOCATION_LOC0;

  /* Enable output on pin - GPIO Port F, Pin 2 */
  GPIO->P[5].MODEL &= ~(_GPIO_P_MODEL_MODE2_MASK);
  GPIO->P[5].MODEL |= GPIO_P_MODEL_MODE2_PUSHPULL;
#else
  /* Set location 1 */
  GPIO->ROUTE = (GPIO->ROUTE & ~(_GPIO_ROUTE_SWLOCATION_MASK)) |GPIO_ROUTE_SWLOCATION_LOC1;
  /* Enable output on pin */
  GPIO->P[2].MODEH &= ~(_GPIO_P_MODEH_MODE15_MASK);
  GPIO->P[2].MODEH |= GPIO_P_MODEH_MODE15_PUSHPULL;
#endif

  /* Enable debug clock AUXHFRCO */
  CMU->OSCENCMD = CMU_OSCENCMD_AUXHFRCOEN;

  /* Wait until clock is ready */
  while (!(CMU->STATUS & CMU_STATUS_AUXHFRCORDY));

  /* Enable trace in core debug */
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  ITM->LAR  = 0xC5ACCE55;
  ITM->TER  = 0x0;
  ITM->TCR  = 0x0;
  TPI->SPPR = 2;
  TPI->ACPR = 0xf;
  ITM->TPR  = 0x0;
  DWT->CTRL = 0x400003FE;
  ITM->TCR  = 0x0001000D;
  TPI->FFCR = 0x00000100;
  ITM->TER  = 0x1;
}

int main(void)
{
  // Initialize Silicon Labs device, system, service(s) and protocol stack(s).
  // Note that if the kernel is present, processing task(s) will be created by
  // this call.
  sl_system_init();

  // Initialize the application. For example, create periodic timer(s) or
  // task(s) if the kernel is present.

  /* Initialize chip */
  CHIP_Init();

  /* Initalizing */
  init();

  setupSWOForPrint();




  while (1) {

    // Do not remove this call: Silicon Labs components process action routine
    // must be called from the super loop.
    sl_system_process_action();

    //For every loop where a button is pressed, update the player position
    turn_player(1, DELTA_TIME); //Examples for now
    move_player(1, 1, DELTA_TIME);

    //Fill transmit buffer with updated values of the game state
    populate_spi_transmit_buffer(0, (uint16_t) 8210, player, game_map, transmitBuffer);


    //For master to send
    USART1_sendBuffer(transmitBuffer, BUFFERSIZE);


    //For slave to write what it receives into SWO
    for (unsigned int i = 0; i < BUFFERSIZE; i++) {
        ITM_SendChar(receiveBuffer[i]+'0');
    }
    ITM_SendChar('\n');
    ITM_SendChar('b');
    ITM_SendChar('\n');


    for (unsigned int i = 0; i < BUFFERSIZE; i++) {
        ITM_SendChar(transmitBuffer[i] +'0');
    }
    ITM_SendChar('\n');
  }
}
