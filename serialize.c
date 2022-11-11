#include <math.h>
#include "serialize.h"

#define M_PI_2 1.57079632679489661923
#define M_PI_4 0.78539816339744830962


uint32_t float_to_fixed(float input)
{
    return (uint32_t)(round(input * (1 << 14))); //18 integral bits, 14 fractional bits
}

void populate_angle_vectors(Player player, uint8_t* transmit_buffer) {
  //Populating buffer with player struct
  //Got rid of vision_angle, as it is not needed in the fpga

  uint32_t idx = 2810;
  float angle = player.vision_angle + M_PI_2;
  for (int i = 0; i < 4; i++) {
    uint32_t x_dir = float_to_fixed(cosf(angle));
    uint32_t y_dir = float_to_fixed(sinf(angle));
    angle -= M_PI_4;
    if (i == 2) {
        //don't include forward dir (already transferred)
        angle += M_PI_4;
    }
    transmit_buffer[idx++] = (x_dir >> 24) & 0xFF;
    transmit_buffer[idx++] = (x_dir >> 16) & 0xFF;
    transmit_buffer[idx++] = (x_dir >> 8) & 0xFF;
    transmit_buffer[idx++] = x_dir & 0xFF;
    transmit_buffer[idx++] = (y_dir >> 24) & 0xFF;
    transmit_buffer[idx++] = (y_dir >> 16) & 0xFF;
    transmit_buffer[idx++] = (y_dir >> 8) & 0xFF;
    transmit_buffer[idx++] = y_dir & 0xFF;
  }

}

void populate_spi_transmit_buffer(uint8_t packet_mode, uint16_t packet_size, Player player, GameBlock current_map[GAME_MAP_SIZE][GAME_MAP_SIZE], uint8_t* transmit_buffer){
  //Setting up header with metadata
  transmit_buffer[0] = (packet_size >> 8) & 0xFF;
  transmit_buffer[1] = packet_size & 0xFF;
  transmit_buffer[2] = packet_mode; //Packet mode 0: send everything, mode 1: send only player struct
  transmit_buffer[3] = 0b01010101; //This is a mystery value that can be useful later

  //Populating buffer with player struct
  //Got rid of vision_angle, as it is not needed in the fpga
  uint32_t x_pos = float_to_fixed(player.x_pos);
  uint32_t y_pos = float_to_fixed(player.y_pos);
  uint32_t x_dir = float_to_fixed(player.x_dir);
  uint32_t y_dir = float_to_fixed(player.y_dir);

  transmit_buffer[4] = (x_pos >> 24) & 0xFF;
  transmit_buffer[5] = (x_pos >> 16) & 0xFF;
  transmit_buffer[6] = (x_pos >> 8) & 0xFF;
  transmit_buffer[7] = x_pos & 0xFF;

  transmit_buffer[8] = (y_pos >> 24) & 0xFF;
  transmit_buffer[9] = (y_pos >> 16) & 0xFF;
  transmit_buffer[10] = (y_pos >> 8) & 0xFF;
  transmit_buffer[11] = y_pos & 0xFF;

  transmit_buffer[12] = (x_dir >> 24) & 0xFF;
  transmit_buffer[13] = (x_dir >> 16) & 0xFF;
  transmit_buffer[14] = (x_dir >> 8) & 0xFF;
  transmit_buffer[15] = x_dir & 0xFF;

  transmit_buffer[16] = (y_dir >> 24) & 0xFF;
  transmit_buffer[17] = (y_dir >> 16) & 0xFF;
  transmit_buffer[18] = (y_dir >> 8) & 0xFF;
  transmit_buffer[19] = y_dir & 0xFF;

  uint32_t idx = 20;

  //Add map to buffer if packet mode is not 1
  if (packet_mode != 1){
    for (int i = 0; i < GAME_MAP_SIZE; i++){ // If map size can vary, we need to have a double loop
      for (int j = 0; j < GAME_MAP_SIZE*2; j+=2){
          transmit_buffer[(j+idx)+(i*GAME_MAP_SIZE*2)] = current_map[i][j/2].state & 0xFF;
          transmit_buffer[(j+idx+1)+(i*GAME_MAP_SIZE*2)] = current_map[i][j/2].texture & 0xFF;
      }
    }
  }
  //Populating buffer with extra directions
  //populate_angle_vectors(player, transmit_buffer);

  //TODO:
  //Put game map into bitstream (done)
  //Find size of buffer to be sent over SPI (done)
  //Add header bytes to the bitstream (done)
  //Test that transmit works (make dummy map) (done)
  //Implement different information in bitstream depending on SPI_PACKET_TYPE (See presentation for details) (done? more to come when fpga is more specified)
  //Implement serializing of texture when texture format is more specified
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
}

//We decided not to use this
/*void populate_spi_transmit_buffer_test_player(Player* player, uint8_t* transmit_buffer){
  TestSendPlayerStruct *send_player = malloc(sizeof(TestSendPlayerStruct));
  send_player->x_pos = float_to_fixed(player->x_pos);
  send_player->y_pos = float_to_fixed(player->y_pos);
  send_player->vision_angle = float_to_fixed(player->vision_angle);
  send_player->x_dir = float_to_fixed(player->x_dir);
  send_player->y_dir = float_to_fixed(player->y_dir);

  //memcpy(transmit_buffer, send_player, sizeof(*send_player));
  memcpy(transmit_buffer, player, sizeof(*player));
}*/
