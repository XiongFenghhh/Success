/*****************************************************************
�汾��V1.0
ʱ�䣺2015-04-17 19:21:08
���ߣ�@hiyangdong
���ܣ�Bruthless Motor Control
�ļ���bruthlessMotor.c
*****************************************************************/
#include "brushlessMotor.h"




/*****************************************************************
������  void BMoror_Init(void)
���ܣ�  Initialize Bruthless Motor
���룺  void
�����  void
���ţ�	
				TIM1_CH1 ----- PA8
	
				TIM1_CH1 ----- PA8
				TIM1_CH2 ----- PA9
���ߣ�	@hiyangdong
�汾��	V0.1
ʱ�䣺	2015-04-11 20:45:53
*****************************************************************/
void BMotor_Init(void)
{
    GPIO_InitTypeDef          gpio;
    TIM_TimeBaseInitTypeDef   tim;
    TIM_OCInitTypeDef         oc;
    uint16_t PrescalerValue = (uint16_t) (SystemCoreClock / 200000) - 1;
	
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOB,ENABLE);
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);

    gpio.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
    gpio.GPIO_Mode = GPIO_Mode_AF;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOE,&gpio);
  
		gpio.GPIO_Pin=GPIO_Pin_6;
		gpio.GPIO_Mode= GPIO_Mode_OUT;
		gpio.GPIO_Speed=GPIO_Speed_100MHz;
		gpio.GPIO_PuPd=GPIO_PuPd_UP;
		GPIO_Init(GPIOB,&gpio);
	
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource5, GPIO_AF_TIM9);
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource6, GPIO_AF_TIM9);
	 
		RCC_TIMCLKPresConfig(RCC_TIMPrescActivated);	
    
	
    tim.TIM_Prescaler = PrescalerValue;
    tim.TIM_CounterMode = TIM_CounterMode_Up;
		//@hiyangdong,2015-04-17 19:06:22,�ϸ�50Hz,5%~10%ռ�ձ�
		tim.TIM_Period = BM_PWM_MAX-1;   																
    tim.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM9,&tim);
    
    oc.TIM_OCMode = TIM_OCMode_PWM2;
    oc.TIM_OutputState = TIM_OutputState_Enable;
    oc.TIM_OutputNState = TIM_OutputState_Disable;
    oc.TIM_Pulse = 200;
    oc.TIM_OCPolarity = TIM_OCPolarity_Low;
    oc.TIM_OCNPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM9,&oc);
    TIM_OC2Init(TIM9,&oc);
    
    TIM_OC1PreloadConfig(TIM9,TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(TIM9,TIM_OCPreload_Enable);
           
    TIM_ARRPreloadConfig(TIM9,ENABLE);
		TIM_SetCompare1(TIM9,200);
	#ifndef Gun
	TIM_SetCompare2(TIM9,200);
	#endif
	#ifdef Gun
	TIM_SetCompare2(TIM9,332);
	#endif
	TIM_Cmd(TIM9,ENABLE);
}
/**
*@description Enable Timer9 
*@para	 none
*@retVal none
*/
void brushLessEnable(void)
{
	TIM_SetCompare1(TIM9,200);
	#ifndef Gun
	TIM_SetCompare2(TIM9,200);
	#endif
	#ifdef Gun
	TIM_SetCompare2(TIM9,332);
	#endif
	
}

/**
*@description Disable Timer9 
*@para	 none
*@retVal none
*/
void brushLessDisable(void)
{
	TIM_SetCompare1(TIM9,200);
	#ifndef Gun
	TIM_SetCompare2(TIM9,200);
	#endif
	#ifdef Gun
	TIM_SetCompare2(TIM9,332);
	#endif
	
}

/*****************************************************************
������  void BMoror_PWM(void)
���ܣ�  PWM control bruthless motor
���룺  channel:1,2;PWM:250~500
�����  void
���ţ�	
���ߣ�	@hiyangdong
�汾��	V0.1
ʱ�䣺	2015-04-11 20:45:53
*****************************************************************/
void BMotor_PWM(uint8_t channel)
{
	uint32_t pwm;
	if(channel==1){
	pwm=getBMPWM()<=0.05*BM_PWM_MAX?\
	0.05*BM_PWM_MAX:getBMPWM()>=0.1*BM_PWM_MAX?\
	0.1*BM_PWM_MAX:getBMPWM();}
#ifndef Gun	
	else
	{
		pwm=getBMPWM()<=0.05*BM_PWM_MAX?\
	0.05*BM_PWM_MAX:getBMPWM()>=0.1*BM_PWM_MAX?\
	0.1*BM_PWM_MAX:getBMPWM()+8;
	}
#endif
	if(pwm>0.05*BM_PWM_MAX)GPIO_ResetBits(GPIOC,GPIO_Pin_10);
	else GPIO_SetBits(GPIOC,GPIO_Pin_10);
	switch(channel)
	{
		case 1:	TIM_SetCompare1(TIM9,pwm);break;
		case 2:	TIM_SetCompare2(TIM9,pwm);break;
		default:TIM_SetCompare1(TIM9,0.05*BM_PWM_MAX);break;
	}
}
