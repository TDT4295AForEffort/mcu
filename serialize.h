#ifndef SERIALIZE_H_
#define SERIALIZE_H_

#include "game.h"
#include "stdint.h"

#define PLAYER_BUFFER_OFFSET 4

typedef struct TestStruct{
  int one;
  int two;
  int three;
  int four;
  int five;
} TestStruct;

void populate_spi_transmit_buffer(/*GameBlock** current_map,*/ Player* player, uint8_t* transmit_buffer);
void populate_spi_transmit_buffer_test(TestStruct* test, uint8_t* transmit_buffer);

#endif /* SERIALIZE_H_ */
