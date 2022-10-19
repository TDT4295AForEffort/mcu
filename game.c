#include "game.h"
#include <math.h>
#include "stdint.h"

void turn_right(Player* player, float amount, float dt) {
  player->vision_angle -= amount * TURN_SPEED * dt;
  player->x_dir = cos(player->vision_angle);
  player->y_dir = sin(player->vision_angle);
}

void turn_left(Player* player, float amount, float dt) {
  player->vision_angle += amount * TURN_SPEED * dt;
  player->x_dir = cos(player->vision_angle);
  player->y_dir = sin(player->vision_angle);
}

void move_forward(Player* player, GameBlock** map, float amount, float dt) {
  float x_new = player->x_pos + amount * MOVE_SPEED * player->x_dir * dt;
  float y_new = player->y_pos + amount * MOVE_SPEED * player->y_dir * dt;
  bool colliding = check_collision(x_new, y_new, map);
  if (!colliding) {
      player->x_pos = x_new;
      player->y_pos = y_new;
  }
}

void move_backward(Player* player, GameBlock** map, float amount, float dt) {
  float x_new = player->x_pos - amount * MOVE_SPEED * player->x_dir * dt;
  float y_new = player->y_pos - amount * MOVE_SPEED * player->y_dir * dt;
  bool colliding = check_collision(x_new, y_new, map);
  if (!colliding) {
      player->x_pos = x_new;
      player->y_pos = y_new;
  }
}

bool check_collision(float x_pos, float y_pos, GameBlock** map) {
  int x_block_pos = x_pos;
  int y_block_pos = y_pos;
  if (x_block_pos < 0 || y_block_pos < 0) {
      return true;
  }
  uint8_t block_state = map[x_block_pos][y_block_pos].state;
  if (block_state != 0) return true;
  return false;
}


void init_map(GameBlock** map) {
  for (int i = 0; i < GAME_MAP_SIZE; i++) {
      //fill borders with wall
      map[0][i].state = 1;
      map[GAME_MAP_SIZE-1][i].state = 1;
      map[i][0].state = 1;
      map[i][GAME_MAP_SIZE-1].state = 1;
  }
  //TODO: fill some random blocks?
}
