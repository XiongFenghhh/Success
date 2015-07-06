#include "time.h"
#include "stm32f4xx_tim.h"

//PWMInCh1:副翼  PWMInCh2:升降舵  PWMInCh3:油门  PWMInCh4:方向舵
uint16_t PWMInCh1=0, PWMInCh2=0, PWMInCh3=0, PWMInCh4=0;

/**************************实现函数********************************************
*函数原型: TIM6_HalfT_INIT
		
*功　　能: 用于确定姿态解算函数中的HalfT		
*******************************************************************************/
void TIM6_HalfT_INIT(void)
{
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); //时钟使能

//	TIM_TimeBaseStructure.TIM_Period = 0xFFFF; //1us计一次数，计到n则为n us
//	TIM_TimeBaseStructure.TIM_Prescaler = 108 - 1; //设置用来作为TIMx时钟频率除数的预分频值，1us计一个数，我的主频跑到108Mhz  
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
//	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

//	TIM_Cmd(TIM6, ENABLE);
}

/**************************实现函数********************************************
*函数原型: TIM7_dt_INIT
		
*功　　能: 用于确定PID控制周期		
*******************************************************************************/
void TIM7_dt_INIT(void)
{
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE); //时钟使能

//	TIM_TimeBaseStructure.TIM_Period = 0xFFFF; //1us计一次数，计到n则为n us
//	TIM_TimeBaseStructure.TIM_Prescaler = 108 - 1; //设置用来作为TIMx时钟频率除数的预分频值，1us计一个数，我的主频跑到108Mhz  
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
//	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

//	TIM_Cmd(TIM7, ENABLE);
}
/*======================================Motor PWM OUT Time3========================================
========================================Motor PWM OUT Time3========================================
========================================Motor PWM OUT Time3=======================================*/

/**************************实现函数************************************
*函数原型: TIM4_PWMOutMotor_Init
*引    脚: PD12对接M1号电机  PD13对接M2号电机  
           PD14对接M3号电机  PD15对接M4号电机		
*功　　能: 为4个电调输送PWMOUT信号，用于控制电机转动		
***********************************************************************/
void TIM4_PWMOutMotor_Init(void)
{
  ;//deleted
}

/**************************实现函数************************************
*函数原型: TIM4_Motor_PWMOutput
		
*功　　能: Tim4输出PWM给电调，DR1、DR2、DR3、DR4为经过PID的MOTOR1 MOTOR2
           MOTOR3 MOTOR4		
***********************************************************************/
void TIM4_Motor_PWMOutput(uint16_t DR1,uint16_t DR2,uint16_t DR3,uint16_t DR4)
{
//	TIM_SetCompare1(TIM4,DR1);
//	TIM_SetCompare2(TIM4,DR2);
//	TIM_SetCompare3(TIM4,DR3);
//	TIM_SetCompare4(TIM4,DR4);
}
/*=========================================================================================*/
/*=========================================================================================*/


/*======================================PWM IN Time2========================================
========================================PWM IN Time2========================================
========================================PWM IN Time2=======================================*/

/**************************实现函数************************************
*函数原型: TIM2_PWMInCap_Init
*引    脚: PA0接收副翼AIL(对应Roll)   PA1接收升降舵ELE(对应Pitch)  
           PA2接收油门THR             PA3接收方向舵RUD(对应Yaw)		
*功　　能: 配置TIM2，用于捕获接收机输出的4个PWM信号，用的是Tim2的
           输入捕获模式，不是PWM输入模式。		
***********************************************************************/
void TIM2_PWMInCap_Init(void)
{	 
	;//deleted
}
/*=========================================================================================*/
/*=========================================================================================*/

float GET_NOWTIME(void)//返回当前tim6计数器值,32位
{
	float temp=0 ;
	static uint32_t now=0; // 采样周期计数 单位 us

 	now = TIM6->CNT;//读高16位时间
   	TIM6->CNT=0;
	temp = (float)now / 2000000.0f;          //换算成秒，再除以2得出采样周期的一半

	return temp;
}

float GET_PIDTIME(void)//返回当前tim7计数器值,32位
{
	float temp_PID=0 ;
	static uint32_t now_PID=0; // 采样周期计数 单位 us

 	now_PID = TIM7->CNT;//读高16位时间
   	TIM7->CNT=0;
	temp_PID = (float)now_PID / 1000000.0f;          //换算成秒

	return temp_PID;
}


void get_ms(unsigned long *time)
{
}

/*=========================================================================================*/
