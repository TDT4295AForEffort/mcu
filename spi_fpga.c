#include "spi_fpga.h"

#include "sl_spidrv_instances.h"
#include "sl_spidrv_fpga_config.h"
#include "app.h"

void sendBufferToFpga(uint8_t* buffer, int size) {
  Ecode_t ret = SPIDRV_MTransmitB(sl_spidrv_fpga_handle, buffer, size);
  if (ret != ECODE_OK) {
      switch (ret){
        case ECODE_EMDRV_SPIDRV_ILLEGAL_HANDLE:
          ITM_SendChar('a');
          break;
        case ECODE_EMDRV_SPIDRV_PARAM_ERROR:
          ITM_SendChar('b');
          break;
        case ECODE_EMDRV_SPIDRV_BUSY:
          ITM_SendChar('c');
          break;
        case ECODE_EMDRV_SPIDRV_MODE_ERROR:
          ITM_SendChar('d');
          break;
      }

    //error
  }else{
      ITM_SendChar('*');
    //not error
  }
}
