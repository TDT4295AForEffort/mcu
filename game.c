#include "game.h"
#include <math.h>

void turn_right(Player* player, float joystick_rigth, float dt) {
  player->vision_angle -= TURN_SPEED * dt * joystick_rigth;
  player->x_dir = cos(player->vision_angle);
  player->y_dir = sin(player->vision_angle);
}

void turn_left(Player* player, float joystick_left, float dt) {
  player->vision_angle += TURN_SPEED * dt * joystick_left;
  player->x_dir = cos(player->vision_angle);
  player->y_dir = sin(player->vision_angle);
}

void move_forward(Player* player, float joystick_forward, float dt) {
  //if(player->x_pos + MOVE_SPEED * player->x_pos * dt * joystick_forward < GAME_MAP_SIZE){
      player->x_pos = player->x_pos + MOVE_SPEED * player->x_pos * dt * joystick_forward;
  //}
  //if(player->y_pos + MOVE_SPEED * player->y_pos * dt * joystick_forward < GAME_MAP_SIZE){
      player->y_pos = player->y_pos + MOVE_SPEED * player->y_pos * dt * joystick_forward;
  //}
}

void move_backward(Player* player, float joystick_backwards, float dt) {
  if(player->x_pos - MOVE_SPEED * player->x_pos * dt * joystick_backwards > 0){
      player->x_pos -= MOVE_SPEED * player->x_pos * dt * joystick_backwards;
  }
  if(player->y_pos - MOVE_SPEED * player->y_pos * dt * joystick_backwards > 0){
      player->y_pos -= MOVE_SPEED * player->y_pos * dt * joystick_backwards;
  }
}

//TODO: Detect collisions

void init_map(GameBlock map[GAME_MAP_SIZE][GAME_MAP_SIZE]) {
  for (int i = 0; i < GAME_MAP_SIZE; i++) {
      //fill borders with wall
      map[0][i].state = 1;
      map[GAME_MAP_SIZE-1][i].state = 1;
      map[i][0].state = 1;
      map[i][GAME_MAP_SIZE-1].state = 1;
  }
  //TODO: fill some random blocks?
}
