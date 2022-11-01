#include "game.h"
#include "stdint.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void turn_player(float amount, float dt) {
  float rot_angle = amount * TURN_SPEED * dt;
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
  //player starts in middle of the map, and looks along x-axis
  player.x_dir = 1.0;
  player.y_dir = 0.0;
  player.x_pos = GAME_MAP_SIZE / 2 - 0.5;
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
      game_map[i][j].state = 1;
    }
  }
}
