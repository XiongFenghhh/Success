

/*****************************************************************
�汾��V1.0
ʱ�䣺2015��4��1�� 21:05:39
���ߣ�@hiyangdong
���ܣ�Encoder and Motor Control
�ļ���Encoder.c
*****************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "Encoder.h"
#include "stm32f4xx_tim.h"
//@hiyangdong
//2015��4��11�� 20:41:50
//����Ҫ�ظ������ˡ�ֻ��Ҫ�޸Ķ�Ӧ���ž�����

#define EncoderStartCNT 0x8000
static int16_t preEncnt[4][2];

/*****************************************************************
������  void Encoder_TIM1_Init(void)
���ܣ�  Initialize Timer1
���룺  void
�����  void
���ţ�	
				TIM1_CH1 ----- PE9
				TIM1_CH2 ----- PE11
���ߣ�	@hiyangdong
�汾��	V0.1
ʱ�䣺	2015-04-11 20:45:53
*****************************************************************/
void Encoder_Init(void)
{
	Encoder_TIM2_Init();
	Encoder_TIM3_Init();
	Encoder_TIM4_Init();
	//Encoder_Start();
	
}

/*****************************************************************
������  void Encoder_TIM1_Init(void)
���ܣ�  Initialize Timer1
���룺  void
�����  void
���ţ�	
				TIM1_CH1 ----- PE9
				TIM1_CH2 ----- PE11
���ߣ�	@hiyangdong
�汾��	V0.1
ʱ�䣺	2015-04-11 20:45:53
*****************************************************************/
void Encoder_Get(void)
{
		preEncnt[0][0]=preEncnt[0][1];
		preEncnt[1][0]=preEncnt[1][1];
		preEncnt[2][0]=preEncnt[2][1];
		preEncnt[3][0]=preEncnt[3][1];
		preEncnt[0][1]=Encoder_Count(TIM5); 
		preEncnt[1][1] =Encoder_Count(TIM2);
		preEncnt[2][1] =Encoder_Count(TIM4);
		preEncnt[3][1] =Encoder_Count(TIM3);
		encoder_cnt[0]=0*preEncnt[0][0]+1*preEncnt[0][1];
		encoder_cnt[1]=0*preEncnt[1][0]+1*preEncnt[1][1];
		encoder_cnt[2]=0*preEncnt[2][0]+1*preEncnt[2][1];
		encoder_cnt[3]=0*preEncnt[3][0]+1*preEncnt[3][1];
		Encoder_Reset(TIM2);
		Encoder_Reset(TIM3);
		Encoder_Reset(TIM4);
		Encoder_Reset(TIM5);
}

/*****************************************************************
������  void Encoder_TIM1_Init(void)
���ܣ�  Initialize Timer1
���룺  void
�����  void
���ţ�	
				TIM1_CH1 ----- PE9
				TIM1_CH2 ----- PE11
���ߣ�	@hiyangdong
�汾��	V0.1
ʱ�䣺	2015-04-11 20:45:53
*****************************************************************/
void Encoder_TIM1_Init(void)
{
	GPIO_InitTypeDef GPIO_TypeDefine;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);

	GPIO_TypeDefine.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_11;
	GPIO_TypeDefine.GPIO_Mode = GPIO_Mode_AF;
	GPIO_TypeDefine.GPIO_OType = GPIO_OType_PP;
	GPIO_TypeDefine.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_TypeDefine.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOE,&GPIO_TypeDefine);

	GPIO_PinAFConfig(GPIOE, GPIO_PinSource9,  GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_TIM1);

	TIM_EncoderInterfaceConfig(TIM1, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	//Reset counter��From 0x8000
	TIM_SetCounter(TIM1,0x8000);
	TIM_Cmd(TIM1, ENABLE);
}

/*****************************************************************
������  void Encoder_TIM2_Init(void)
���ܣ�  Initialize Timer2
���룺  void
�����  void
���ţ�	
				TIM2_CH1 ----- PA5
				TIM2_CH2 ----- PB3
���ߣ�	@hiyangdong
�汾��	V0.1
ʱ�䣺	2015-04-11 20:45:53
*****************************************************************/
void Encoder_TIM2_Init(void)
{
	GPIO_InitTypeDef GPIO_TypeDefine;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM5,ENABLE);

	GPIO_TypeDefine.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_TypeDefine.GPIO_Mode = GPIO_Mode_AF;
	GPIO_TypeDefine.GPIO_OType = GPIO_OType_PP;
	GPIO_TypeDefine.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_TypeDefine.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA,&GPIO_TypeDefine);

	GPIO_TypeDefine.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOB,&GPIO_TypeDefine);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5,  GPIO_AF_TIM2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0,  GPIO_AF_TIM5);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource1,  GPIO_AF_TIM5);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3,  GPIO_AF_TIM2);

	TIM_TimeBaseInitStructure.TIM_Prescaler = 0; 
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; 
    TIM_TimeBaseInitStructure.TIM_Period = 65535;	
    TIM_TimeBaseInitStructure.TIM_ClockDivision = 0; 
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure); 
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	//Reset counter��From 0x8000
	TIM_SetCounter(TIM2,0x8000);
	TIM_SetCounter(TIM5,0x8000);
	
}

/*****************************************************************
������  void Encoder_TIM3_Init(void)
���ܣ�  Initialize Timer3
���룺  void
�����  void
���ţ�	
				TIM3_CH1 ----- PA6
				TIM3_CH3 ----- PA7
���ߣ�	@hiyangdong
�汾��	V0.1
ʱ�䣺	3015-04-11 30:45:53
*****************************************************************/
void Encoder_TIM3_Init(void)
{
	GPIO_InitTypeDef GPIO_TypeDefine;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);

	GPIO_TypeDefine.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_TypeDefine.GPIO_Mode = GPIO_Mode_AF;
	GPIO_TypeDefine.GPIO_OType = GPIO_OType_PP;
	GPIO_TypeDefine.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_TypeDefine.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA,&GPIO_TypeDefine);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6,  GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7,  GPIO_AF_TIM3);

	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	//Reset counter��From 0x8000
	TIM_SetCounter(TIM3,0x8000);
	
}


/*****************************************************************
������  void Encoder_TIM4_Init(void)
���ܣ�  Initialize Timer4
���룺  void
�����  void
���ţ�	
				TIM4_CH1 ----- PD12
				TIM4_CH4 ----- PD13
���ߣ�	@hiyangdong
�汾��	V0.1
ʱ�䣺	4015-04-11 40:45:54
*****************************************************************/
void Encoder_TIM4_Init(void)
{
	GPIO_InitTypeDef GPIO_TypeDefine;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);

	GPIO_TypeDefine.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
	GPIO_TypeDefine.GPIO_Mode = GPIO_Mode_AF;
	GPIO_TypeDefine.GPIO_OType = GPIO_OType_PP;
	GPIO_TypeDefine.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_TypeDefine.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOD,&GPIO_TypeDefine);

	GPIO_PinAFConfig(GPIOD, GPIO_PinSource12,  GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource13,  GPIO_AF_TIM4);
            //?????????? 
     
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	//Reset counter��From 0x8000
	TIM_SetCounter(TIM4,0x8000);
	
}

/*****************************************************************
������  void Encoder_TIM5_Init(void)
���ܣ�  Initialize Timer5
���룺  void
�����  void
���ţ�	
				TIM5_CH1 ----- PA0
				TIM5_CH2 ----- PA1
���ߣ�	@hiyangdong
�汾��	V0.1
ʱ�䣺	5015-05-11 50:55:55
*****************************************************************/
void Encoder_TIM5_Init(void)
{
	GPIO_InitTypeDef GPIO_TypeDefine;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);

	GPIO_TypeDefine.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_TypeDefine.GPIO_Mode = GPIO_Mode_AF;
	GPIO_TypeDefine.GPIO_OType = GPIO_OType_PP;
	GPIO_TypeDefine.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_TypeDefine.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&GPIO_TypeDefine);
	

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0,  GPIO_AF_TIM5);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1,  GPIO_AF_TIM5);
	
	TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	//Reset counter��From 0x8000

	TIM_SetCounter(TIM5,0x8000);
	
}

/*****************************************************************
������  void Encoder_Start(void)
���ܣ�  Start Timer
���룺  TIMx
�����  void
���ţ�	
���ߣ�	@hiyangdong
�汾��	V0.1
ʱ�䣺	5015-05-11 50:55:55
*****************************************************************/
void Encoder_Start()
{
	
	TIM_Cmd(TIM5, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
	TIM_Cmd(TIM4, ENABLE);
}	
/**
*@description disable Timer2,3,4,5
*@para none
*@retVal none
*/
void Encoder_Disable()
{
	TIM_Cmd(TIM5, DISABLE);
	TIM_Cmd(TIM2, DISABLE);
	TIM_Cmd(TIM3, DISABLE);
	TIM_Cmd(TIM4, DISABLE);	
}

/*****************************************************************
������  void Encoder_Reset(void)
���ܣ�  Reset Timer
���룺  TIMx
�����  void
���ţ�	
���ߣ�	@hiyangdong
�汾��	V0.1
ʱ�䣺	5015-05-11 50:55:55
*****************************************************************/
void Encoder_Reset(TIM_TypeDef* TIMx)
{
	TIMx->CNT = EncoderStartCNT;
}	

/*****************************************************************
������  uint16_t Encoder_Count(TIM_TypeDef* TIMx)
���ܣ�  Get Timer CNT
���룺  TIMx
�����  void
���ţ�	
���ߣ�	@hiyangdong
�汾��	V0.1
ʱ�䣺	5015-05-11 50:55:55
*****************************************************************/
uint16_t Encoder_Count(TIM_TypeDef* TIMx)
{
	return (uint16_t) TIMx->CNT - EncoderStartCNT;
}	


