#ifndef SERIALIZE_H_
#define SERIALIZE_H_

#include "game.h"
#include "stdint.h"
#include <string.h>
#include <stdlib.h>

#define HEADER_BYTES 4
#define PLAYER_BYTES 16
#define MAP_BYTES GAME_MAP_SIZE * GAME_MAP_SIZE * 2
#define ANGLES_BYTES 32
#define ENEMIES_BYTES NUM_ENEMIES * 8
#define BUFFERSIZE HEADER_BYTES+PLAYER_BYTES+MAP_BYTES+ANGLES_BYTES+ENEMIES_BYTES //Size of header+player+map
//#define PLAYER_BUFFER_OFFSET 4
#define SPI_PACKET_TYPE 0

typedef struct TestStruct{
  int one;
  int two;
  int three;
  int four;
  int five;
} TestStruct;

typedef struct TestSendPlayerStruct{
  uint32_t x_pos;
  uint32_t y_pos;
  uint32_t vision_angle;
  uint32_t x_dir;
  uint32_t y_dir;
} TestSendPlayerStruct;


/*typedef struct Metadata_send{
  uint8_t packet_mode;
  uint8_t packet_size;
}Metadata_send;

typedef struct Player_send{
  uint32_t pos[2];
  uint32_t dir[2];
}Player_send;

typedef struct GameBlock_send{
  uint8_t state; //0=air, 1=wall
  uint8_t texture_code;
}GameBlock_send;

typedef struct Texture_send{
  uint8_t texture_code;
  uint16_t texture_data[];
}Texture_send;

typedef struct GameState_send{
  Metadata_send header;
  Player_send player;
  GameBlock_send map[];
};*/
int32_t float_to_fixed(float input);

void populate_spi_transmit_buffer(uint8_t packet_mode, uint16_t packet_size, Player player, GameBlock current_map[GAME_MAP_SIZE][GAME_MAP_SIZE], Enemy enemies[NUM_ENEMIES], uint8_t* transmit_buffer);
void populate_spi_transmit_buffer_test(TestStruct* test, uint8_t* transmit_buffer);
//void populate_spi_transmit_buffer_test_player(Player* player, uint8_t* transmit_buffer);

#endif /* SERIALIZE_H_ */
