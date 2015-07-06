/*****************************************************************
版本：V1.0
时间：2015-04-17 19:21:08
作者：@hiyangdong
功能：Bruthless Motor Control
文件：bruthlessMotor.h
*****************************************************************/

#include "stepper.h"

//A-AB-B-BC-C-CD-D-DA
uint8_t  StepperArray[8]={0x01,0x03,0x02,0x06,0x04,0x0c,0x08,0x09}; 

/*****************************************************************
函数：  void Stepper_Init(void)
功能：  Initialize Stepper
输入：  void
输出：  void
引脚：	GPIOD:PD0,PD1,PD2,PD3
				TIM1_CH2 ----- PA9
作者：	@hiyangdong
版本：	V0.1
时间：	2015-04-11 20:45:53
*****************************************************************/
void Stepper_Init(void)
{
	GPIO_InitTypeDef gpio;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);

	gpio.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	gpio.GPIO_Mode = GPIO_Mode_OUT;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOD,&gpio);
}
/**
*@description disable stepper
*@para none
*@retVal none
*/
void stepperDisabble(void)
{
	GPIO_ResetBits(GPIOD,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3);
}


/*****************************************************************
函数：  void Stepper_Ctrl(void)
功能：  Stepper control
输入：  speed:延时时间,500~5000,建议2000us
				mode :输入一或者二;
输出：  void
引脚：	
作者：	@hiyangdong
版本：	V0.1
时间：	2015-04-17 19:43:22
*****************************************************************/
void	Stepper_Ctrl(uint8_t i)
{ 	
	//j = i * mode;
//	for(uint8_t j=0;j<8;j++)	
	Stepper_Write(GPIOD,StepperArray[i]);     	
}

/*****************************************************************
函数：  void Stepper_Write(void)
功能：  write GPIOs
输入：  GPIOx:引脚;value:输入值;
输出：  void
引脚：	
作者：	@hiyangdong
版本：	V0.1
时间：	2015-04-11 20:45:53
*****************************************************************/
void Stepper_Write(GPIO_TypeDef* GPIOx, uint16_t value)
{
  GPIOx->ODR = (0xfff0&GPIOx->ODR)|(0x000f&value);
}
