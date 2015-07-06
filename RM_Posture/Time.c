#include "time.h"
#include "stm32f4xx_tim.h"

//PWMInCh1:����  PWMInCh2:������  PWMInCh3:����  PWMInCh4:�����
uint16_t PWMInCh1=0, PWMInCh2=0, PWMInCh3=0, PWMInCh4=0;

/**************************ʵ�ֺ���********************************************
*����ԭ��: TIM6_HalfT_INIT
		
*��������: ����ȷ����̬���㺯���е�HalfT		
*******************************************************************************/
void TIM6_HalfT_INIT(void)
{
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); //ʱ��ʹ��

//	TIM_TimeBaseStructure.TIM_Period = 0xFFFF; //1us��һ�������Ƶ�n��Ϊn us
//	TIM_TimeBaseStructure.TIM_Prescaler = 108 - 1; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ��1us��һ�������ҵ���Ƶ�ܵ�108Mhz  
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
//	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

//	TIM_Cmd(TIM6, ENABLE);
}

/**************************ʵ�ֺ���********************************************
*����ԭ��: TIM7_dt_INIT
		
*��������: ����ȷ��PID��������		
*******************************************************************************/
void TIM7_dt_INIT(void)
{
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE); //ʱ��ʹ��

//	TIM_TimeBaseStructure.TIM_Period = 0xFFFF; //1us��һ�������Ƶ�n��Ϊn us
//	TIM_TimeBaseStructure.TIM_Prescaler = 108 - 1; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ��1us��һ�������ҵ���Ƶ�ܵ�108Mhz  
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
//	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

//	TIM_Cmd(TIM7, ENABLE);
}
/*======================================Motor PWM OUT Time3========================================
========================================Motor PWM OUT Time3========================================
========================================Motor PWM OUT Time3=======================================*/

/**************************ʵ�ֺ���************************************
*����ԭ��: TIM4_PWMOutMotor_Init
*��    ��: PD12�Խ�M1�ŵ��  PD13�Խ�M2�ŵ��  
           PD14�Խ�M3�ŵ��  PD15�Խ�M4�ŵ��		
*��������: Ϊ4���������PWMOUT�źţ����ڿ��Ƶ��ת��		
***********************************************************************/
void TIM4_PWMOutMotor_Init(void)
{
  ;//deleted
}

/**************************ʵ�ֺ���************************************
*����ԭ��: TIM4_Motor_PWMOutput
		
*��������: Tim4���PWM�������DR1��DR2��DR3��DR4Ϊ����PID��MOTOR1 MOTOR2
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

/**************************ʵ�ֺ���************************************
*����ԭ��: TIM2_PWMInCap_Init
*��    ��: PA0���ո���AIL(��ӦRoll)   PA1����������ELE(��ӦPitch)  
           PA2��������THR             PA3���շ����RUD(��ӦYaw)		
*��������: ����TIM2�����ڲ�����ջ������4��PWM�źţ��õ���Tim2��
           ���벶��ģʽ������PWM����ģʽ��		
***********************************************************************/
void TIM2_PWMInCap_Init(void)
{	 
	;//deleted
}
/*=========================================================================================*/
/*=========================================================================================*/

float GET_NOWTIME(void)//���ص�ǰtim6������ֵ,32λ
{
	float temp=0 ;
	static uint32_t now=0; // �������ڼ��� ��λ us

 	now = TIM6->CNT;//����16λʱ��
   	TIM6->CNT=0;
	temp = (float)now / 2000000.0f;          //������룬�ٳ���2�ó��������ڵ�һ��

	return temp;
}

float GET_PIDTIME(void)//���ص�ǰtim7������ֵ,32λ
{
	float temp_PID=0 ;
	static uint32_t now_PID=0; // �������ڼ��� ��λ us

 	now_PID = TIM7->CNT;//����16λʱ��
   	TIM7->CNT=0;
	temp_PID = (float)now_PID / 1000000.0f;          //�������

	return temp_PID;
}


void get_ms(unsigned long *time)
{
}

/*=========================================================================================*/
