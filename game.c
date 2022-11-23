#include "game.h"
#include "stdint.h"
#include "utils.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void turn_player(float amount, float dt) {
  float rot_angle = amount * TURN_SPEED * dt;
  player.vision_angle += rot_angle;
  player.x_dir = cosf(player.vision_angle);
  player.y_dir = sinf(player.vision_angle);
}

void move_player(float move_x, float move_y, float dt) {
  float player_left_x = -player.y_dir;
  float player_left_y = player.x_dir;
  float dx = MOVE_SPEED * dt * (move_x * player.x_dir + move_y * player_left_x);
  float dy = MOVE_SPEED * dt * (move_x * player.y_dir + move_y * player_left_y);
  float x_new = player.x_pos + dx;
  float y_new = player.y_pos + dy;
  int x = x_new;
  int y = y_new;
  if (game_map[x][y].state != 0) {
      int x_player = player.x_pos;
      int y_player = player.y_pos;
      if (x != x_player) dx = 0.0;
      if (y != y_player) dy = 0.0;
  }
  player.x_pos += dx;
  player.y_pos += dy;
  check_enemy_collision();
}

void check_enemy_collision() { // Check if enemy is less than 0.5 away from you
  for (int i = 0; i < NUM_ENEMIES; i++) {
    if (fabs(enemies[i].x_pos - player.x_pos) < 0.1 &&
        fabs(enemies[i].y_pos - player.y_pos) < 0.1) {
      game_over(); // reset game state to initial
    }
  }
}

void init_game() {
  init_map();
  init_player();
  init_enemies();
}

void game_over() { init_game(); }

// Make sure you are not so close to a wall so that you clip through it
bool check_block_collision(float x_pos, float y_pos) {
  int x_block_pos = x_pos;
  int y_block_pos = y_pos;
  if (x_block_pos < 0 || y_block_pos < 0 || x_block_pos >= GAME_MAP_SIZE ||
      y_block_pos >= GAME_MAP_SIZE) {
    return true;
  }
  float fract_x = x_pos - x_block_pos;
  float fract_y = y_pos - y_block_pos;

  if (game_map[x_block_pos][y_block_pos].state) {
    return true;
  }
  // check collisions with padding
  if (fract_x < COLLISION_MARGIN && game_map[x_block_pos - 1][y_block_pos].state != 0) {
    return true;
  }
  if (fract_x > (1-COLLISION_MARGIN) && game_map[x_block_pos + 1][y_block_pos].state != 0) {
    return true;
  }
  if (fract_y > COLLISION_MARGIN && game_map[x_block_pos][y_block_pos - 1].state != 0) {
    return true;
  }
  if (fract_x > (1-COLLISION_MARGIN) && game_map[x_block_pos][y_block_pos + 1].state != 0) {
    return true;
  }

  return false;
}

void modify_block(uint8_t state) {

  float x_ray = 0.1 * player.x_dir;
  float y_ray = 0.1 * player.y_dir;
  float x_pos = player.x_pos;
  float y_pos = player.y_pos;
  while (1) {
      int x = x_pos;
      int y = y_pos;
      if (x == 0 || x == GAME_MAP_SIZE-1 || y == 0 || y == GAME_MAP_SIZE-1) {
          x_pos -= x_ray;
          y_pos -= y_ray;
          break;
      }
      if (game_map[x][y].state == 1) {
          break;
      }
      x_pos += x_ray;
      y_pos += y_ray;
  }
  int x = x_pos;
  int y = y_pos;
  if (state != 0) {
      x_pos -= x_ray;
      y_pos -= y_ray;
      x = x_pos;
      y = y_pos;
  }
  game_map[x][y].state = state;
}

void destroy_block() { modify_block(0); }

void place_block() { modify_block(1); }

void init_player() {
  player.x_dir = 1.0;
  player.y_dir = 0.0;
  player.x_pos = GAME_MAP_SIZE * randomFloat();
  player.y_pos = GAME_MAP_SIZE * randomFloat();
  while (check_block_collision(player.x_pos, player.y_pos)) {
    player.x_pos = GAME_MAP_SIZE * randomFloat();
    player.y_pos = GAME_MAP_SIZE * randomFloat();
  }
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
      if (randomFloat() > 0.9) {
        game_map[i][j].state = 1;
      }
    }
  }
}

void init_enemies() {
  for (int i = 0; i < NUM_ENEMIES; i++) {
    enemies[i].x_pos = GAME_MAP_SIZE * randomFloat();
    enemies[i].y_pos = GAME_MAP_SIZE * randomFloat();
    while (check_block_collision(enemies[i].x_pos, enemies[i].y_pos)) {
      enemies[i].x_pos = GAME_MAP_SIZE * randomFloat();
      enemies[i].y_pos = GAME_MAP_SIZE * randomFloat();
    }
    enemies[i].x_dir = 2.0 * randomFloat() - 1.0;
    enemies[i].y_dir = 2.0 * randomFloat() - 1.0;
  }
}

void move_enemies(float dt) {
  for (int i = 0; i < NUM_ENEMIES; i++) {
    float x_new = enemies[i].x_pos + enemies[i].x_dir * MOVE_SPEED/2 * dt;
    float y_new = enemies[i].y_pos + enemies[i].y_dir * MOVE_SPEED/2 * dt;
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
