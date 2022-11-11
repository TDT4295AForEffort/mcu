#ifndef __GAME_H
#define __GAME_H

#include "stdint.h"
#include <stdbool.h>
#define GAME_MAP_SIZE 64
#define NUM_ENEMIES 1
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

typedef struct Enemy{
   float x_pos;
   float y_pos;
} Enemy;

void turn_player(float amount, float dt);

void move_player(float move_x, float move_y, float dt);

bool check_collision(float x_pos, float y_pos);

void destroy_block();

void place_block();

//allocates memory and fills in initial state of map
void init_map();

void init_player();

void init_enemies();
void move_enemies(float dt);

extern GameBlock game_map[GAME_MAP_SIZE][GAME_MAP_SIZE];
extern Player player;
extern Enemy enemies[NUM_ENEMIES];

#endif
