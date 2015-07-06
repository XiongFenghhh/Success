#include "BatholithTimer.h"
static uint8_t deadTime;
static int piancha1=0;
static int piancha2=0;
static int piancha3=0;
static int piancha4=0;

uint8_t getDeadTime(){return deadTime;}
void setDeadTime(uint8_t dt){deadTime=dt;}
/*
*Function: RMTIM_PWM_Config
*Description: 
*1.Config Timer1 and Timer 8 to output pwm wave
*2.use GPIOE8,9,10,11--------Timer 1;
   use Pc7,8  Pb0,1----------Timer 8;
*3.Timer8_ch2(pc7,pb0)------------Right Front
   Timer8_ch3(pc8,pb1)------------left Front
   Timer1_ch1(pe8,pe9)------------left Behind
   Timer1_ch2(pe10,pe11)-----------Right Behind
*Calls: RMTIM1_Config(),RMTIM8_Config()
*/
void RmBatholicTIM_PWM_Config(){
RmBatholicTIM1_Config();
RmBatholicTIM8_Config();
batholithPWMEnable();
}
void RmBatholicTIM1_Config(){
 	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_BDTRInitTypeDef bdStructure;
	uint16_t PrescalerValue = (uint16_t) (SystemCoreClock / 160000000) - 1;
	
	/* TIM8 clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
  /* GPIOD clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	
	
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9|GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8|GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOE,GPIO_Pin_8);
	GPIO_ResetBits(GPIOE,GPIO_Pin_10);
    
 	GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_TIM1);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_TIM1); 

	RCC_TIMCLKPresConfig(RCC_TIMPrescActivated);
		
 
  /* Compute the prescaler value */

	TIM_DeInit(TIM1);
  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 9999;
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	

  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState=TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNPolarity=TIM_OCPolarity_Low;
  TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_OCNIdleState=TIM_OCNIdleState_Set;
	
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	
  	
	TIM_ARRPreloadConfig(TIM1, ENABLE);
  TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
	
}
void RmBatholicTIM8_Config(){
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	/* Compute the prescaler value */
	uint16_t PrescalerValue = (uint16_t) (SystemCoreClock / 160000000) - 1;
	GPIO_InitTypeDef GPIO_InitStructure;
	/* TIM4 clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);

  /* GPIOD clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
  

	
	 GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7 | GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	

	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);
	GPIO_ResetBits(GPIOB,GPIO_Pin_1);


	GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_TIM8);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM8); 

	RCC_TIMCLKPresConfig(RCC_TIMPrescActivated);
	TIM_DeInit(TIM8);
  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 9999;
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);
	

  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState=TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNPolarity=TIM_OCPolarity_Low;
  TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_OCNIdleState=TIM_OCNIdleState_Set;
	
	TIM_OC2Init(TIM8, &TIM_OCInitStructure);
	TIM_OC3Init(TIM8, &TIM_OCInitStructure);

	 
	
	TIM_ARRPreloadConfig(TIM8, ENABLE);
	TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);

}
/*
*@date 2015.4.14
*add protection for pwm because PID parameter isn't perfect yet
*/
void batholithSetPwm(double pwm,GPIO_TypeDef* GPIOx, uint16_t GPIO_PinSource)
{
	if(pwm>=0)
	{
		GPIO_ResetBits(GPIOx,GPIO_PinSource);
		
	}else 
	{
		GPIO_SetBits(GPIOx,GPIO_PinSource);
	}
}

void batholithResetPwm(){
	TIM_SetCompare2(TIM8,0);
	TIM_SetCompare3(TIM8,0);
	TIM_SetCompare2(TIM1,0);
	TIM_SetCompare1(TIM1,0);	
	me.isPWMallowed=0;
}
void setPiancha1(int temp){piancha1=temp;}
void setPiancha2(int temp){piancha2=temp;}
void setPiancha3(int temp){piancha3=temp;}
void setPiancha4(int temp){piancha4=temp;}

/**
*@description enable TIMER1,8
*@para none
*@retVal none
*/
void batholithPWMEnable(void){
	TIM_Cmd(TIM1,ENABLE);
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
	TIM_Cmd(TIM8,ENABLE);
	TIM_CtrlPWMOutputs(TIM8,ENABLE);
	batholithResetPwm();	
}

/**
*@description disable Timer1,8
*@para none
*@retVal none
*/
void batholithPWMDisable(void){
	batholithResetPwm();
	GPIO_ResetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1);
	GPIO_ResetBits(GPIOE,GPIO_Pin_8|GPIO_Pin_10);
}

