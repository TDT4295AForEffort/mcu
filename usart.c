#include "em_device.h"
#include "usart.h"
#include "em_gpio.h"

/******************************************************************************
 * @brief sends data using USART2
 * @param txBuffer points to data to transmit
 * @param bytesToSend bytes will be sent
 *****************************************************************************/
void USART2_sendBuffer(char* txBuffer, int bytesToSend)
{
  USART_TypeDef *uart = USART2;
  int           ii;

  /* Sending the data */
  for (ii = 0; ii < bytesToSend;  ii++)
  {
    /* Waiting for the usart to be ready */
    while (!(uart->STATUS & USART_STATUS_TXBL)) ;

    if (txBuffer != 0)
    {
      /* Writing next byte to USART */
      uart->TXDATA = *txBuffer;
      txBuffer++;
    }
    else
    {
      uart->TXDATA = 0;
    }
  }

  /*Waiting for transmission of last byte */
  while (!(uart->STATUS & USART_STATUS_TXC)) ;
}

void USART1_sendBuffer(char* txBuffer, int bytesToSend)
{
  USART_TypeDef *uart = USART1;
  int           ii;

  /* Sending the data */
  for (ii = 0; ii < bytesToSend;  ii++)
  {
    /* Waiting for the usart to be ready */
    while (!(uart->STATUS & USART_STATUS_TXBL)) ;

    if (txBuffer != 0)
    {
      /* Writing next byte to USART */
      uart->TXDATA = *txBuffer;
      txBuffer++;
    }
    else
    {
      uart->TXDATA = 0;
    }
  }

  /*Waiting for transmission of last byte */
  while (!(uart->STATUS & USART_STATUS_TXC)) ;
}