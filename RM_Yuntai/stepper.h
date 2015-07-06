/*****************************************************************
版本：V1.0
时间：2015-04-17 11:08:07
作者：@hiyangdong
功能：Stepper control
文件：Encoder.h
*****************************************************************/

#ifndef STEPPER_H__
#define STEPPER_H__

#include "RMVariables.h"
extern void Stepper_Init(void);
extern void	Stepper_Ctrl(uint8_t);
void stepperDisable(void);
void Stepper_Write(GPIO_TypeDef* GPIOx, uint16_t value);

#endif
