#include "game.h"
#include "stdint.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

uint32_t seed = 0;

// A single iteration of Bob Jenkins' One-At-A-Time hashing algorithm.
uint32_t hash(uint32_t* x) {
  *x += (*x << 10);
  *x ^= (*x >> 6);
  *x += (*x << 3);
  *x ^= (*x >> 11);
  *x += (*x << 15);
  return *x;
}

// Construct a float with half-open range [0:1] using low 23 bits.
// All zeroes yields 0.0, all ones yields the next smallest representable value below 1.0.
float floatConstruct(uint32_t m) {
  const uint32_t ieeeMantissa = 0x007FFFFF; // binary32 mantissa bitmask
  const uint32_t ieeeOne = 0x3F800000; // 1.0 in IEEE binary32

  m &= ieeeMantissa;                     // Keep only mantissa bits (fractional part)
  m |= ieeeOne;                          // Add fractional part to 1.0

    //int to float
  float  f = *(float*)(&m);       // Range [1:2]
  return f - 1.0;                        // Range [0:1]
}

float randomFloat() {
  return floatConstruct(hash(&seed));
}

void turn_player(float amount, float dt) {
  float rot_angle = -amount * TURN_SPEED * dt;
  player.vision_angle += rot_angle;
  player.x_dir = cosf(player.vision_angle);
  player.y_dir = sinf(player.vision_angle);
}

void move_player(float move_x, float move_y, float dt) {
  float player_left_x = -player.y_dir;
  float player_left_y = player.x_dir;
  float x_new = player.x_pos + MOVE_SPEED * dt * (move_x * player_left_x + move_y * player.x_dir);
  float y_new = player.y_pos + MOVE_SPEED * dt * (move_x * player_left_y + move_y * player.y_dir);
  bool colliding = check_collision(x_new, y_new);
  if (!colliding) {
    player.x_pos = x_new;
    player.y_pos = y_new;
  }
}

//Make sure you are not so close to a wall so that you clip through it. (no closer than 0.125)
bool check_collision(float x_pos, float y_pos) {
  int x_block_pos = x_pos;
  int y_block_pos = y_pos;
  if (x_block_pos < 0 || y_block_pos < 0 || x_block_pos > GAME_MAP_SIZE || y_block_pos > GAME_MAP_SIZE) {
    return true;
  }
  float fract_x = x_pos-x_block_pos;
  float fract_y = y_pos-y_block_pos;

  if (game_map[x_block_pos][y_block_pos].state) {
      return true;
  }
  //check collisions with padding
  if (fract_x < 0.125 && game_map[x_block_pos-1][y_block_pos].state != 0){
      return true;
  }
  if (fract_x > 0.875 && game_map[x_block_pos+1][y_block_pos].state != 0){
      return true;
  }
  if (fract_y > 0.125 && game_map[x_block_pos][y_block_pos-1].state != 0){
      return true;
  }
  if (fract_x > 0.875 && game_map[x_block_pos][y_block_pos+1].state != 0){
      return true;
  }
  return false;
}

void modify_block(uint8_t state) {
  int x_block_pos = player.x_pos;
  int y_block_pos = player.y_pos;
  if (fabs(player.x_dir) > fabs(player.y_dir)) {
      x_block_pos += (player.x_dir > 0) - (player.x_dir < 0);
      if (x_block_pos > 0 && x_block_pos < GAME_MAP_SIZE) {
          game_map[x_block_pos][y_block_pos].state = state;
      }
  }
  else {
      y_block_pos += (player.y_dir > 0) - (player.y_dir < 0);
      if (y_block_pos > 0 && y_block_pos < GAME_MAP_SIZE) {
         game_map[x_block_pos][y_block_pos].state = state;
      }
  }
}

void destroy_block() {
  modify_block(0);
}

void place_block() {
  modify_block(1);
}

void init_player() {
  player.x_dir = 1.0;
  player.y_dir = 0.0;
  player.x_pos = 1.5;
  player.y_pos = GAME_MAP_SIZE / 2 - 0.5;
  player.vision_angle = 0.0;
}

void init_map() {
  for (int i = 0; i < GAME_MAP_SIZE; i++) {
    //fill borders with wall
    game_map[0][i].state = 1;
    game_map[GAME_MAP_SIZE-1][i].state = 1;
    game_map[i][0].state = 1;
    game_map[i][GAME_MAP_SIZE-1].state = 1;
  }
  //fill in blocks in checker pattern
  for (int i = 2; i < GAME_MAP_SIZE; i += 2) {
    for (int j = 2; j < GAME_MAP_SIZE; j += 2) {
      //game_map[i][j].state = 1;
    }
  }
}

void init_enemies() {
  for (int i = 0; i < NUM_ENEMIES; i++) {
      enemies[i].x_pos = GAME_MAP_SIZE * randomFloat();
      enemies[i].y_pos = GAME_MAP_SIZE * randomFloat();
  }
}

void move_enemies(float dt) {
  for (int i = 0; i < NUM_ENEMIES; i++) {
      float x_diff = player.x_pos - enemies[i].x_pos;
      float y_diff = player.y_pos - enemies[i].y_pos;
      if (fabs(x_diff) > fabs(y_diff)) {
          enemies[i].x_pos += x_diff * 0.5 * MOVE_SPEED * dt;
      }
      else {
          enemies[i].y_pos += y_diff * 0.5 * MOVE_SPEED * dt;
      }
  }
}
