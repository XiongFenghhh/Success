#ifndef __USART2_H__
#define __USART2_H__

#include "stm32f4xx.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "RMVariables.h"
void USART2_Configuration(void);
void USART2_SendChar(unsigned char b);
//void USART2_IRQHandler(void);
//called by main()

#endif
