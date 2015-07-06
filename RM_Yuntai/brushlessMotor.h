/*****************************************************************
版本：V1.0
时间：2015-04-17 19:21:08
作者：@hiyangdong
功能：Bruthless Motor Control
文件：bruthlessMotor.h
*****************************************************************/

#ifndef _BRUTHLESSMOTOR_H__
#define _BRUTHLESSMOTOR_H__
#include "RMVariables.h"
#ifdef __cplusplus
 extern "C" {
	 #endif
#include "stm32f4xx.h"

extern void BMotor_Init(void);
extern void BMotor_PWM(uint8_t channel);
void brushLessEnable(void);
void brushLessDisable(void);	 
#ifdef __cplusplus
}
#endif
#endif 
