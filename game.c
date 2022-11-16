#include "game.h"
#include "stdint.h"
#include "utils.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void turn_player(float amount, float dt) {
  float rot_angle = -amount * TURN_SPEED * dt;
  player.vision_angle += rot_angle;
  player.x_dir = cosf(player.vision_angle);
  player.y_dir = sinf(player.vision_angle);
}

void move_player(float move_x, float move_y, float dt) {
  float player_left_x = -player.y_dir;
  float player_left_y = player.x_dir;
  float x_new =
      player.x_pos +
      MOVE_SPEED * dt * (move_x * player_left_x + move_y * player.x_dir);
  float y_new =
      player.y_pos +
      MOVE_SPEED * dt * (move_x * player_left_y + move_y * player.y_dir);
  bool colliding = check_block_collision(x_new, y_new);
  if (!colliding) {
    player.x_pos = x_new;
    player.y_pos = y_new;
  }
  check_enemy_collision();
}

void check_enemy_collision() { // Check if enemy is less than 0.5 away from you
  for (int i = 0; i < NUM_ENEMIES; i++) {
    if (fabs(enemies[i].x_pos - player.x_pos) < 0.5 ||
        fabs(enemies[i].y_pos - player.y_pos) < 0.5) {
      game_over(); // reset game state to initial
    }
  }
}

void game_over() {
  init_map();
  init_player();
  init_enemies();
}

// Make sure you are not so close to a wall so that you clip through it. (no
// closer than 0.125)
bool check_block_collision(float x_pos, float y_pos) {
  int x_block_pos = x_pos;
  int y_block_pos = y_pos;
  if (x_block_pos < 0 || y_block_pos < 0 || x_block_pos > GAME_MAP_SIZE ||
      y_block_pos > GAME_MAP_SIZE) {
    return true;
  }
  float fract_x = x_pos - x_block_pos;
  float fract_y = y_pos - y_block_pos;

  if (game_map[x_block_pos][y_block_pos].state) {
    return true;
  }
  // check collisions with padding
  if (fract_x < 0.125 && game_map[x_block_pos - 1][y_block_pos].state != 0) {
    return true;
  }
  if (fract_x > 0.875 && game_map[x_block_pos + 1][y_block_pos].state != 0) {
    return true;
  }
  if (fract_y > 0.125 && game_map[x_block_pos][y_block_pos - 1].state != 0) {
    return true;
  }
  if (fract_x > 0.875 && game_map[x_block_pos][y_block_pos + 1].state != 0) {
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
  } else {
    y_block_pos += (player.y_dir > 0) - (player.y_dir < 0);
    if (y_block_pos > 0 && y_block_pos < GAME_MAP_SIZE) {
      game_map[x_block_pos][y_block_pos].state = state;
    }
  }
}

void destroy_block() { modify_block(0); }

void place_block() { modify_block(1); }

void init_player() {
  player.x_dir = 1.0;
  player.y_dir = 0.0;
  player.x_pos = 2.5;
  player.y_pos = GAME_MAP_SIZE / 2 - 0.5;
  player.vision_angle = 0.0;
}

void init_map() {
  for (int i = 0; i < GAME_MAP_SIZE; i++) {
    // fill borders with wall
    game_map[0][i].state = 1;
    game_map[GAME_MAP_SIZE - 1][i].state = 1;
    game_map[i][0].state = 1;
    game_map[i][GAME_MAP_SIZE - 1].state = 1;
  }
  // fill in random blocks
  for (int i = 1; i < GAME_MAP_SIZE - 1; i++) {
    for (int j = 1; j < GAME_MAP_SIZE - 1; j++) {
      if (randomFloat() > 0.8) {
        game_map[i][j].state = 1;
      }
    }
  }
}

void init_enemies() {
  for (int i = 0; i < NUM_ENEMIES; i++) {
    enemies[i].x_pos = GAME_MAP_SIZE * randomFloat();
    enemies[i].y_pos = GAME_MAP_SIZE * randomFloat();
    enemies[i].x_dir = 2.0 * randomFloat() - 1.0;
    enemies[i].y_dir = 2.0 * randomFloat() - 1.0;
  }
}

void move_enemies(float dt) {
  for (int i = 0; i < NUM_ENEMIES; i++) {
    float x_new = enemies[i].x_pos + enemies[i].x_dir * MOVE_SPEED * dt;
    float y_new = enemies[i].y_pos + enemies[i].y_dir * MOVE_SPEED * dt;
    if (!check_block_collision(x_new, y_new)) {
      enemies[i].x_pos = x_new;
      enemies[i].y_pos = y_new;
    }
    if (randomFloat() > 0.99) {
      float x_diff = player.x_pos - enemies[i].x_pos;
      float y_diff = player.y_pos - enemies[i].y_pos;
      float distance = sqrt(x_diff * x_diff + y_diff * y_diff);
      enemies[i].x_dir = x_diff / distance;
      enemies[i].y_dir = y_diff / distance;
    }
  }
}
