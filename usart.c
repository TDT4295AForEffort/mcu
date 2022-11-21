#include "usart.h"
#include "em_device.h"
#include "em_gpio.h"

/******************************************************************************
 * @brief sends data using USART2
 * @param txBuffer points to data to transmit
 * @param bytesToSend bytes will be sent
 *****************************************************************************/
void USART2_sendBuffer(char *txBuffer, int bytesToSend) {
  USART_TypeDef *uart = USART2;
  int ii;

  /* Sending the data */
  for (ii = 0; ii < bytesToSend; ii++) {
    /* Waiting for the usart to be ready */
    while (!(uart->STATUS & USART_STATUS_TXBL))
      ;

    if (txBuffer != 0) {
      /* Writing next byte to USART */
      uart->TXDATA = *txBuffer;
      txBuffer++;
    } else {
      uart->TXDATA = 0;
    }
  }

  /*Waiting for transmission of last byte */
  while (!(uart->STATUS & USART_STATUS_TXC))
    ;
}

void USART1_sendBuffer(uint8_t *txBuffer, int bytesToSend) {
  USART_TypeDef *uart = USART1;
  int ii;

  //GPIO_PinModeSet(gpioPortD, 3, 4, 0); /* CS Devboard*/
  GPIO_PinModeSet(gpioPortB, 8, 4, 0);   /* CS PCB*/
  //GPIO_PinModeSet(gpioPortC, 5, 4, 0);   /* CS Debug*/
  /* Sending the data */
  for (ii = 0; ii < bytesToSend; ii++) {
    /* Waiting for the usart to be ready */
    while (!(uart->STATUS & USART_STATUS_TXBL))
      ;

    if (txBuffer != 0) {
      /* Writing next byte to USART */
      uart->TXDATA = *txBuffer;
      txBuffer++;
    } else {
      uart->TXDATA = 0;
      //GPIO_PinModeSet(gpioPortD, 3, 4, 1); /* CS */
      GPIO_PinModeSet(gpioPortB, 8, 4, 1);   /* CS PCB*/
      //GPIO_PinModeSet(gpioPortC, 5, 4, 0);   /* CS Debug*/
    }
  }

  /*Waiting for transmission of last byte */
  while (!(uart->STATUS & USART_STATUS_TXC))
    ;
}
