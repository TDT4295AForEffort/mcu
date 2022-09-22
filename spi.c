#include "EFM32-Library/v2/emdrv/spidrv/inc/spidrv.h"
#include "spi.h"

// Handle for the FPGA output.
SPIDRV_HandleData_t handleData;
SPIDRV_Handle_t handle = &handleData;

//Make function to send bitstream
void send_game_state(struct fpga_package *original_package){
  uint8_t bitstream[PACKAGE_SIZE] = {0}; //To set up bitstream

  //Populate the bitstream here by converting the fpga_package to a fpga_package_send
  bitstream[0] = original_package->player_info->x_pos;
  bitstream[1] = original_package->player_info->y_pos;
  bitstream[2] = original_package->player_info->vision_angle;

  //change 2d array into 1d array
  for (int i = 0; i < GAME_MAP_SIZE; i++){
      for (int j = 0; j < GAME_MAP_SIZE; j++){
          bitstream[(j+3)+(i*GAME_MAP_SIZE)] = original_package->game_map[i][j];
      }
  }

  //Transmit the bitstream to the fpga
  SPIDRV_MTransmitB(handle, bitstream, sizeof(bitstream));
}

int main( void ) //This should probably be in its own file
{
  //uint8_t buffer[10];
  SPIDRV_Init_t initData = SPIDRV_MASTER_USART2;

  // Initialize a SPI driver instance
  SPIDRV_Init( handle, &initData );

  // Transmit data using a blocking transmit function
  //SPIDRV_MTransmitB( handle, buffer, 10 ); //This one should only be called in the send_game_state function

  // Transmit data using a callback to catch transfer completion.
  //SPIDRV_MTransmit( handle, buffer, 10, TransferComplete ); //Don't ned this right now
}
