#include "game.h"
#include <math.h>

void turn_right(Player* player, float dt) {
  player->vision_angle -= TURN_SPEED * dt;
  player->x_dir = cos(player->vision_angle);
  player->y_dir = sin(player->vision_angle);
}

void turn_left(Player* player, float dt) {
  player->vision_angle += TURN_SPEED * dt;
  player->x_dir = cos(player->vision_angle);
  player->y_dir = sin(player->vision_angle);
}

void move_forward(Player* player, float dt) {
  player->x_pos += MOVE_SPEED * player->x_dir * dt;
  player->y_pos += MOVE_SPEED * player->y_dir * dt;
}

void move_backward(Player* player, float dt) {
  player->x_pos -= MOVE_SPEED * player->x_dir * dt;
  player->y_pos -= MOVE_SPEED * player->y_dir * dt;
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
