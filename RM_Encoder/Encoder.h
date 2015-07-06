/*****************************************************************
�汾��V1.0
ʱ�䣺2015��4��1�� 21:05:39
���ߣ�@hiyangdong
���ܣ�Encoder and Motor Control
�ļ���Encoder.c
*****************************************************************/

#ifndef ENCODER_H__
#define ENCODER_H__

#include "stm32f4xx.h"
#include "RMVariables.h"

extern void Encoder_Init(void);
extern void Encoder_Get(void);
extern void Encoder_Start(void);
void Encoder_Disable(void);
extern void Encoder_Reset(TIM_TypeDef* TIMx);
extern uint16_t Encoder_Count(TIM_TypeDef* TIMx);

void Encoder_TIM1_Init(void);
void Encoder_TIM2_Init(void);
void Encoder_TIM3_Init(void);
void Encoder_TIM4_Init(void);
void Encoder_TIM5_Init(void);
//s16 Encoder_Calc_Average_Speed_Calc_Rot_Speed(void);

void Encoder_Init(void);
int16_t getEncoder0(void);
int16_t getEncoder1(void);
int16_t getEncoder2(void);
int16_t getEncoder3(void);
#endif
