#ifndef _TIME_H_
#define _TIME_H_

#include "stm32f4xx.h" 

extern uint16_t PWMInCh1, PWMInCh2, PWMInCh3, PWMInCh4;

void TIM6_HalfT_INIT(void);
void TIM7_dt_INIT(void);
void TIM4_PWMOutMotor_Init(void);
void TIM4_Motor_PWMOutput(uint16_t DR1,uint16_t DR2,uint16_t DR3,uint16_t DR4);
void TIM2_PWMInCap_Init(void);
float GET_NOWTIME(void);
float GET_PIDTIME(void);
void get_ms(unsigned long *time);

#endif

