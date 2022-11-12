#include "spi_fpga.h"

#include "sl_spidrv_instances.h"
#include "sl_spidrv_fpga_config.h"
#include "app.h"

void sendBufferToFpga(uint8_t* buffer, int size) {
  Ecode_t ret = SPIDRV_MTransmitB(sl_spidrv_fpga_handle, buffer, size);
  if (ret != ECODE_OK) {
      ITM_SendChar('@');
    //error
  }
}
