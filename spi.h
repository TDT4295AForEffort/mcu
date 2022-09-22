#define GAME_MAP_SIZE = 64;

struct game_block{ //Might have to move this somewhere else later
  uint8_t state; //What are we going to store in here actually?
  uint8_t texture;
};

struct player{
  float x_pos;
  float y_pos;
  float vision_angle;
};

struct fpga_package {
  struct player player_info;
  struct game_block game_map[GAME_MAP_SIZE][GAME_MAP_SIZE];
  //More stuff as we need it


  //struct fpga_package_header header; //Do we need a header? Find out!
  //vec4_t verts[NUM_VERTS];
  //line_t lines[NUM_LINES];
  //mat4_t mat;

};

struct game_map_send{
  uint16_t game_blocks[GAME_MAP_SIZE*GAME_MAP_SIZE];
};

struct fpga_package_send {
  struct player player_info;
  struct game_map_send game_map;

  //struct fpga_package_header header;
  //fpga_point_t points[NUM_VERTS];
  //line_t lines[NUM_LINES];
  //mat4_send_t mat;
};

#define PACKAGE_SIZE (sizeof(struct fpga_package_send))
void send_game_state(struct fpga_package *drawcall);
