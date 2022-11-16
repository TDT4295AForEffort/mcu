#include "utils.h"
#include "em_adc.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_device.h"
#include "em_emu.h"
#include "em_gpio.h"
#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void setupSWOForPrint(void) {
  /* Enable GPIO clock. */
  CMU->HFPERCLKEN0 |= CMU_HFPERCLKEN0_GPIO;

  /* Enable Serial wire output pin */
  GPIO->ROUTE |= GPIO_ROUTE_SWOPEN;

#if defined(_EFM32_GIANT_FAMILY) || defined(_EFM32_LEOPARD_FAMILY) ||          \
    defined(_EFM32_WONDER_FAMILY)
  /* Set location 0 */
  GPIO->ROUTE = (GPIO->ROUTE & ~(_GPIO_ROUTE_SWLOCATION_MASK)) |
                GPIO_ROUTE_SWLOCATION_LOC0;

  /* Enable output on pin - GPIO Port F, Pin 2 */
  GPIO->P[5].MODEL &= ~(_GPIO_P_MODEL_MODE2_MASK);
  GPIO->P[5].MODEL |= GPIO_P_MODEL_MODE2_PUSHPULL;
#else
  /* Set location 1 */
  GPIO->ROUTE = (GPIO->ROUTE & ~(_GPIO_ROUTE_SWLOCATION_MASK)) |
                GPIO_ROUTE_SWLOCATION_LOC1;
  /* Enable output on pin */
  GPIO->P[2].MODEH &= ~(_GPIO_P_MODEH_MODE15_MASK);
  GPIO->P[2].MODEH |= GPIO_P_MODEH_MODE15_PUSHPULL;
#endif

  /* Enable debug clock AUXHFRCO */
  CMU->OSCENCMD = CMU_OSCENCMD_AUXHFRCOEN;

  /* Wait until clock is ready */
  while (!(CMU->STATUS & CMU_STATUS_AUXHFRCORDY))
    ;

  /* Enable trace in core debug */
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  ITM->LAR = 0xC5ACCE55;
  ITM->TER = 0x0;
  ITM->TCR = 0x0;
  TPI->SPPR = 2;
  TPI->ACPR = 0xf;
  ITM->TPR = 0x0;
  DWT->CTRL = 0x400003FE;
  ITM->TCR = 0x0001000D;
  TPI->FFCR = 0x00000100;
  ITM->TER = 0x1;
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
  for (int i = 0; i < NUM_ENEMIES; i++) {
    print_str("enemy nr ");
    ITM_SendChar('0' + i);
    ITM_SendChar(' ');
    gcvt(enemies[i].x_pos, 6, buf);
    print_str(buf);
    ITM_SendChar(' ');
    gcvt(enemies[i].y_pos, 6, buf);
    print_str(buf);
    ITM_SendChar('\n');
  }

  for (int i = 0; i < GAME_MAP_SIZE; i++) {
    for (int j = 0; j < GAME_MAP_SIZE; j++) {
      uint8_t is_enemy = 0;
      for (int k = 0; k < NUM_ENEMIES; k++) {
        int enemy_x = enemies[k].x_pos;
        int enemy_y = enemies[k].y_pos;
        is_enemy = (i == enemy_x && j == enemy_y);
      }

      if (i == player_x && j == player_y) {
        ITM_SendChar('P');
      } else if (is_enemy) {
        ITM_SendChar('E');
      } else if (i == cursor_x && j == cursor_y) {
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

// A single iteration of Bob Jenkins' One-At-A-Time hashing algorithm.
uint32_t hash(uint32_t *x) {
  *x += (*x << 10);
  *x ^= (*x >> 6);
  *x += (*x << 3);
  *x ^= (*x >> 11);
  *x += (*x << 15);
  return *x;
}

// Construct a float with half-open range [0:1] using low 23 bits.
// All zeroes yields 0.0, all ones yields the next smallest representable value
// below 1.0.
float floatConstruct(uint32_t m) {
  const uint32_t ieeeMantissa = 0x007FFFFF; // binary32 mantissa bitmask
  const uint32_t ieeeOne = 0x3F800000;      // 1.0 in IEEE binary32

  m &= ieeeMantissa; // Keep only mantissa bits (fractional part)
  m |= ieeeOne;      // Add fractional part to 1.0

  // int to float
  float f = *(float *)(&m); // Range [1:2]
  return f - 1.0;           // Range [0:1]
}

float randomFloat() { return floatConstruct(hash(&seed)); }

void srandom(uint32_t new_seed) { seed = new_seed; }
