#include "math.h"
#include "serialize.h"

uint32_t float_to_fixed(float input)
{
    return (uint32_t)(round(input * (1 << 16)));
}

void populate_spi_transmit_buffer(/*GameBlock** current_map,*/ Player* player, uint8_t* transmit_buffer){
   //Testing first populating player struct
  uint32_t x_pos = float_to_fixed(player->x_pos);
  uint32_t y_pos = float_to_fixed(player->y_pos);
  uint32_t vision_angle = float_to_fixed(player->vision_angle);
  uint32_t x_dir = float_to_fixed(player->x_dir);
  uint32_t y_dir = float_to_fixed(player->y_dir);

  transmit_buffer[0] = (x_pos >> 24) & 0xFF;
  transmit_buffer[1] = (x_pos >> 16) & 0xFF;
  transmit_buffer[2] = (x_pos >> 8) & 0xFF;
  transmit_buffer[3] = x_pos & 0xFF;

  transmit_buffer[4] = (y_pos >> 24) & 0xFF;
  transmit_buffer[5] = (y_pos >> 16) & 0xFF;
  transmit_buffer[6] = (y_pos >> 8) & 0xFF;
  transmit_buffer[7] = y_pos & 0xFF;

  transmit_buffer[8] = (vision_angle >> 24) & 0xFF;
  transmit_buffer[9] = (vision_angle >> 16) & 0xFF;
  transmit_buffer[10] = (vision_angle >> 8) & 0xFF;
  transmit_buffer[11] = vision_angle & 0xFF;

  transmit_buffer[12] = (x_dir >> 24) & 0xFF;
  transmit_buffer[13] = (x_dir >> 16) & 0xFF;
  transmit_buffer[14] = (x_dir >> 8) & 0xFF;
  transmit_buffer[15] = x_dir & 0xFF;

  transmit_buffer[16] = (y_dir >> 24) & 0xFF;
  transmit_buffer[17] = (y_dir >> 16) & 0xFF;
  transmit_buffer[18] = (y_dir >> 8) & 0xFF;
  transmit_buffer[19] = y_dir & 0xFF;

  /*transmit_buffer[0] = float_to_fixed(player->x_dir);
  transmit_buffer[PLAYER_BUFFER_OFFSET] = float_to_fixed(player->y_dir);
  transmit_buffer[PLAYER_BUFFER_OFFSET*2] = float_to_fixed(player->vision_angle);
  transmit_buffer[PLAYER_BUFFER_OFFSET*3] = float_to_fixed(player->x_dir);
  transmit_buffer[PLAYER_BUFFER_OFFSET*4] = float_to_fixed(player->y_dir);*/

}

void populate_spi_transmit_buffer_test(/*GameBlock** current_map,*/ TestStruct* test, uint8_t* transmit_buffer){
   //Testing populating test struct, we do it 8 bit at a time
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

  /*transmit_buffer[PLAYER_BUFFER_OFFSET] = test->two;
  transmit_buffer[PLAYER_BUFFER_OFFSET*2] = test->three;
  transmit_buffer[PLAYER_BUFFER_OFFSET*3] = test->four;;
  transmit_buffer[PLAYER_BUFFER_OFFSET*4] = test->five;*/

}
