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
#include <stdio.h>
#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_gpio.h"
#include "em_adc.h"
#include "adc.h"
#include "game.h"
#include "random.h"
#include "serialize.h"
#include "spi_fpga.h"


uint8_t transmitBuffer[BUFFERSIZE];
uint8_t receiveBuffer[BUFFERSIZE];

GameBlock game_map[GAME_MAP_SIZE][GAME_MAP_SIZE];
Player player;
Enemy enemies[NUM_ENEMIES];

uint32_t seed;
int c;
uint32_t sample_x, sample_y;

void app_init(void) {
  c = 0;
  /* Enabling clock to USART 0 and 1*/
    CMU_ClockEnable(cmuClock_USART0, true);
    CMU_ClockEnable(cmuClock_USART1, true);
    CMU_ClockEnable(cmuClock_GPIO, true);

    setupSWOForPrint();

    initADC();

    srandom(69420);
    init_map();
    init_player();
    init_enemies();
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

void print_str(char str[]) {
  char c;
  int i = 0;
  do {
      c = str[i++];
      ITM_SendChar(c);
  } while (c != '\0');
}

void print_gamestate() {
    ITM_SendChar('\n');
    int player_x = player.x_pos;
    int player_y = player.y_pos;
    int cursor_x = player_x + (int)(2.5 * player.x_dir);
    int cursor_y = player_y + (int)(2.5 * player.y_dir);
    char buf[100];
    gcvt(enemies[0].x_pos, 6, buf);
    print_str(buf);
    ITM_SendChar(' ');
    gcvt(enemies[0].y_pos, 6, buf);
    print_str(buf);
    ITM_SendChar('\n');
    for (int i = 0; i < GAME_MAP_SIZE; i++) {
        for (int j = 0; j < GAME_MAP_SIZE; j++) {
            bool is_enemy = false;
            for (int k = 0; k < NUM_ENEMIES; k++) {
                int enemy_x = enemies[k].x_pos;
                int enemy_y = enemies[k].y_pos;
                is_enemy = (i == enemy_x && j == enemy_y);
            }

            if (i == player_x && j == player_y) {
                ITM_SendChar('P');
            }
            else if (is_enemy) {
                ITM_SendChar('E');
            }
            else if (i == cursor_x && j == cursor_y) {
                ITM_SendChar('+');
            }

            else if (game_map[i][j].state != 0) {
                ITM_SendChar('#');
            }

            else {

                ITM_SendChar(' ');
            }
            ITM_SendChar(' ');
        }
        ITM_SendChar('\n');
    }
    ITM_SendChar('\n');
}



void app_process_action(void)
{
    c++;
    // Sample joystick in X-direction
    sample_x = sampleJoystick(adcSingleInputCh2);

    // Sample joystick in Y-direction
    sample_y = sampleJoystick(adcSingleInputCh3);
    //printJoystickSample(sample_x);
    //ITM_SendChar(' ');
    //printJoystickSample(sample_y);
    //ITM_SendChar('\n');
    const float dt = 0.001;
    move_player(0.0, convertSample(sample_x), dt);
    turn_player(convertSample(sample_y), dt);
    move_enemies(dt);
    // Do not remove this call: Silicon Labs components process action routine
    // must be called from the super loop.
    sl_system_process_action();

    //Fill transmit buffer with updated values of the game state
    populate_spi_transmit_buffer(0, (uint16_t) BUFFERSIZE, player, game_map, transmitBuffer);
    //For master to send
    sendBufferToFpga(transmitBuffer, BUFFERSIZE);
    //ITM_SendChar('\n');
    //memset(receiveBuffer, '\0', BUFFERSIZE);
    // Application process.
    if (c > 100) {
        //printJoystickSample(sample_x);
        //ITM_SendChar(' ');
        //printJoystickSample(sample_y);
        //ITM_SendChar('\n');
        print_gamestate();
        c = 0;
    }
}
