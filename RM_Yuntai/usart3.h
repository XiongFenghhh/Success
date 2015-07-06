#ifndef __USART3_H__
#define __USART3_H__

#include "stm32f4xx.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "RMVariables.h"

void USART3_Configuration(void);
void USART3_SendChar(unsigned char b);
void USART3_IRQHandler(void);
//called by main()
int16_t getYunTaiAdjustYaw(void);
int16_t getYunTaiAdjustPitch(void);
void setMotoParameter(void);
void resetIsReceive(void);
uint8_t getIsReceive(void);
void RS232_VisualScope( USART_TypeDef* USARTx, u8 *pWord, u16 Len );
void ANO_DT_Send_Status(float angle_rol, float angle_pit, float angle_yaw, s32 alt, u8 fly_model, u8 armed);
unsigned char HtoEs_Attitude_Data_Generate(void);
extern uint8_t HtoEs_OutPut_Buffer[15];
#endif
