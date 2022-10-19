#ifndef __GAME_H
#define __GAME_H

#include "stdint.h"
#include <stdbool.h>
#define GAME_MAP_SIZE 64
//TODO: find nice values for these
#define TURN_SPEED 0.5
#define MOVE_SPEED 0.8

#define MAP_SIZE_MAX_X 8
#define MAP_SIZE_MAX_Y 8
#define TEXTURE_SIZE_MAX_X 8
#define TEXTURE_SIZE_MAX_Y 8
#define NUMBER_OF_TEXTURES 0

typedef struct GameBlock{
  // for now, state = 0 means air, state = 1 means wall
  uint8_t state; //What are we going to store in here actually?
  uint8_t texture;
} GameBlock;

typedef struct Player{
  float x_pos;
  float y_pos;
  float vision_angle;
  float x_dir;
  float y_dir;
} Player;

// Forslag til send-data
typedef struct Player_Send{
  uint32_t pos[2];
  uint32_t direction[2]; // Normalized
  uint32_t fov;
} Player_Send;

typedef struct Texture_Send{
  uint8_t texture_code;
  uint8_t texture_x_size;
  uint8_t texture_y_size;
  uint16_t texture_data[TEXTURE_SIZE_MAX_Y*TEXTURE_SIZE_MAX_X];
} Texture_Send;

typedef struct MetaData_Send{
  uint8_t packet_mode;
  uint8_t package_size;
  uint8_t map_x_size;
  uint8_t map_y_size;
  uint8_t number_of_textures;
} MetaData_Send;

typedef struct GameState_Send{
  MetaData_Send header;
  Player_Send player_data;
  GameBlock map[MAP_SIZE_MAX_X*MAP_SIZE_MAX_Y];
  Texture_Send Textures[NUMBER_OF_TEXTURES];
} GameState_Send;

void turn_right(Player*, float amount, float dt);

void turn_left(Player*, float amount, float dt);

void move_forward(Player*, GameBlock**, float amount, float dt);

void move_backward(Player*, GameBlock**, float amount, float dt);

bool check_collision(float x_pos, float y_pos, GameBlock**);

//map is a 2D array of gameblocks. initialized like this: GameBlock map[GAME_MAP_SIZE][GAME_MAP_SIZE]
void init_map(GameBlock**);

#endif
