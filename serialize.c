#include "serialize.h"
#include <math.h>

#define M_PI_2 1.57079632679489661923
#define M_PI_4 0.78539816339744830962

int32_t float_to_fixed(float input) {
  return (int32_t)(
      round(input * (1 << 14))); // 18 integral bits, 14 fractional bits
}

//Calculate additional direction vectors for the fpga
int populate_angle_vectors(Player player, uint8_t *transmit_buffer, int current_idx) {

  uint32_t idx = current_idx;
  float angle[] = {player.vision_angle + M_PI_2, player.vision_angle + M_PI_4, player.vision_angle - M_PI_4, player.vision_angle - M_PI_2};
  for (int i = 0; i < 4; i++) {
    int32_t x_dir = float_to_fixed(cosf(angle[i]));
    int32_t y_dir = float_to_fixed(sinf(angle[i]));

    transmit_buffer[idx++] = (x_dir >> 24) & 0xFF;
    transmit_buffer[idx++] = (x_dir >> 16) & 0xFF;
    transmit_buffer[idx++] = (x_dir >> 8) & 0xFF;
    transmit_buffer[idx++] = x_dir & 0xFF;
    transmit_buffer[idx++] = (y_dir >> 24) & 0xFF;
    transmit_buffer[idx++] = (y_dir >> 16) & 0xFF;
    transmit_buffer[idx++] = (y_dir >> 8) & 0xFF;
    transmit_buffer[idx++] = y_dir & 0xFF;
}
  return idx;
}

void populate_spi_transmit_buffer(
    uint8_t packet_mode, uint16_t packet_size, Player player,
    GameBlock current_map[GAME_MAP_SIZE][GAME_MAP_SIZE],
    Enemy enemies[NUM_ENEMIES], uint8_t *transmit_buffer) {
  int idx = 0;

  // Setting up header with metadata
  transmit_buffer[idx++] = (packet_size >> 8) & 0xFF;
  transmit_buffer[idx++] = packet_size & 0xFF;
  transmit_buffer[idx++] = packet_mode; // Packet mode 0: send everything, mode 1: send only player struct
  transmit_buffer[idx++] = 0b01010101; // This is a mystery value that can be useful later

  // Populating buffer with player struct
  // The Raycaster used the player values a bit unexpectedly, so we do a quick fix here
  int32_t x_pos = float_to_fixed(player.x_pos);
  int32_t y_pos = float_to_fixed(player.y_pos);
  int32_t x_dir = float_to_fixed(player.x_dir);
  int32_t y_dir = float_to_fixed(player.y_dir);

  transmit_buffer[idx++] = (x_pos >> 24) & 0xFF;
  transmit_buffer[idx++] = (x_pos >> 16) & 0xFF;
  transmit_buffer[idx++] = (x_pos >> 8) & 0xFF;
  transmit_buffer[idx++] = x_pos & 0xFF;

  transmit_buffer[idx++] = (y_pos >> 24) & 0xFF;
  transmit_buffer[idx++] = (y_pos >> 16) & 0xFF;
  transmit_buffer[idx++] = (y_pos >> 8) & 0xFF;
  transmit_buffer[idx++] = y_pos & 0xFF;

  transmit_buffer[idx++] = (x_dir >> 24) & 0xFF;
  transmit_buffer[idx++] = (x_dir >> 16) & 0xFF;
  transmit_buffer[idx++] = (x_dir >> 8) & 0xFF;
  transmit_buffer[idx++] = x_dir & 0xFF;

  transmit_buffer[idx++] = (y_dir >> 24) & 0xFF;
  transmit_buffer[idx++] = (y_dir >> 16) & 0xFF;
  transmit_buffer[idx++] = (y_dir >> 8) & 0xFF;
  transmit_buffer[idx++] = y_dir & 0xFF;

  // Add map to buffer if packet mode is not 1
  if (packet_mode != 1) {
    uint8_t bitmask = 0;
    uint8_t bitcounter = 1;
    int bm_idx = idx;
    for (int i = 0; i < GAME_MAP_SIZE; i++) { // If map size can vary, we need to have a double loop
      for (int j = 0; j < GAME_MAP_SIZE; j++) {
        //Old code that sent both state and texture, not as a bit map
        //transmit_buffer[idx++] = current_map[i][j].state & 0xFF;
        //transmit_buffer[idx++] = current_map[i][j].texture & 0xFF;

        //stack all states (1 bit) into buffer
        idx+=2; //To allign with expected length in fpga
        bitmask |= ((current_map[GAME_MAP_SIZE-1-j][i].state & 0x1) << (8 - bitcounter)); //Take state of each block into bit map
        if (bitcounter == 8) {
            transmit_buffer[bm_idx++] = bitmask; //Actually sets the values of the bit map
            bitcounter = 0;
            bitmask = 0;
        }
        bitcounter++;
      }
    }
  }
  // Populating buffer with extra directions
  idx = populate_angle_vectors(player, transmit_buffer, idx);

  for (int i = 0; i < NUM_ENEMIES; i++) {
    int32_t x_pos = float_to_fixed(enemies[i].x_pos);
    int32_t y_pos = float_to_fixed(enemies[i].y_pos);

    transmit_buffer[idx++] = (x_pos >> 24) & 0xFF;
    transmit_buffer[idx++] = (x_pos >> 16) & 0xFF;
    transmit_buffer[idx++] = (x_pos >> 8) & 0xFF;
    transmit_buffer[idx++] = x_pos & 0xFF;

    transmit_buffer[idx++] = (y_pos >> 24) & 0xFF;
    transmit_buffer[idx++] = (y_pos >> 16) & 0xFF;
    transmit_buffer[idx++] = (y_pos >> 8) & 0xFF;
    transmit_buffer[idx++] = y_pos & 0xFF;
  }
}

void populate_spi_transmit_buffer_test(TestStruct *test, uint8_t *transmit_buffer) {
  // Testing populating test struct, we do it 8 bit at a time
  transmit_buffer[0] = (test->one >> 24) & 0xFF;
  transmit_buffer[1] = (test->one >> 16) & 0xFF;
  transmit_buffer[2] = (test->one >> 8) & 0xFF;
  transmit_buffer[3] = test->one & 0xFF;

  transmit_buffer[4] = (test->two >> 24) & 0xFF;
  transmit_buffer[5] = (test->two >> 16) & 0xFF;
  transmit_buffer[6] = (test->two >> 8) & 0xFF;
  transmit_buffer[7] = test->two & 0xFF;

  transmit_buffer[8] = (test->three >> 24) & 0xFF;
  transmit_buffer[9] = (test->three >> 16) & 0xFF;
  transmit_buffer[10] = (test->three >> 8) & 0xFF;
  transmit_buffer[11] = test->three & 0xFF;

  transmit_buffer[12] = (test->four >> 24) & 0xFF;
  transmit_buffer[13] = (test->four >> 16) & 0xFF;
  transmit_buffer[14] = (test->four >> 8) & 0xFF;
  transmit_buffer[15] = test->four & 0xFF;

  transmit_buffer[16] = (test->five >> 24) & 0xFF;
  transmit_buffer[17] = (test->five >> 16) & 0xFF;
  transmit_buffer[18] = (test->five >> 8) & 0xFF;
  transmit_buffer[19] = test->five & 0xFF;
}
