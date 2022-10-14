#ifndef __GAME_H
#define __GAME_H

#include "stdint.h"
#define GAME_MAP_SIZE 64
//TODO: find nice values for these
#define TURN_SPEED 0.5
#define MOVE_SPEED 0.8

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

void turn_right(Player*, float);

void turn_left(Player*, float);

void move_forward(Player*, float);

void move_backward(Player*, float);

//map is a 2D array of gameblocks. initialized like this: GameBlock map[GAME_MAP_SIZE][GAME_MAP_SIZE]
void init_map(GameBlock[GAME_MAP_SIZE][GAME_MAP_SIZE]);

#endif
