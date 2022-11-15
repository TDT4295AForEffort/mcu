#include "utils.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_gpio.h"
#include "em_adc.h"
#include "game.h"

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
