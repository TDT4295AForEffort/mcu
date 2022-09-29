#ifndef __USART_H
#define __USART_H

#include <stdbool.h>
#include "em_device.h"

#define NO_RX                    0
#define NO_TX                    NO_RX

#define USART0_NUM               0
#define USART1_NUM               1
#define USART2_NUM               2

#define GPIO_POS0                0
#define GPIO_POS1                1
#define GPIO_POS2                2


void USART2_sendBuffer(char*, int);
void USART1_sendBuffer(char*, int);

#endif