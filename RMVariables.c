#include "RMVariables.h"
struct MotorEngine me;
struct RC_Ctl_t RC_Ctl;
MPU6050_RAW_DATA    MPU6050_Raw_Data; 
MPU6050_REAL_DATA   MPU6050_Real_Data;
int16_t encoder_cnt[4]={0};

int16_t shootStopCount=0;
double setXSpeed;
double realXSpeed;
double setYSpeed;
double realYSpeed;
double realWSpeed;
double preX;
double preY;
uint8_t yunTaiProtectionMode;
double Kp=8;
double Ki=0;
double Kd=0.9;

unsigned char pWord[10];
unsigned char vscope_en = 1;
  
void delay_us(u32 n)
{
	u8 j;
	while(n--)
	for(j=0;j<42;j++);		    
}

void delay_ms(uint16_t t){
int i;
	for( i=0;i<t;i++)
	{
		int a=42000; //at 168MHz 42000 is ok
		while(a--);
	}
}

void RM_Init(void){
	RmBatholicTIM_PWM_Config();
	MotorEngine_Config();
	RC_Init();
	Stepper_Init();
	BMotor_Init();
	CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_4tq,CAN_BS1_9tq,3,CAN_Mode_Normal);
	USART3_Configuration();
	Encoder_Init();
	
	//following functions is mainly for debugging purpose
	
}
void RM_InterruptInit(void){

		if (SysTick_Config(SystemCoreClock / 200))
  { 
    /* Capture error */ 
    while (1);
  }
}
/**
*@function RM_SystemSwitch()
*@description Turn on or shut down the system
*@para mode:If mode is 1 turn on the system. If mode is 0 shut down the system.
*@retVal none
*/
void RM_SystemSwitch(uint8_t mode){
if(mode==1&&me.isRun==0)
	{
		
		batholithPWMEnable();
		MotorEngineReset();
		batholithResetPwm();
		Encoder_Start();
		RC_Reset();
		setXSpeed=0;
		setYSpeed=0;
		
		yunTaiProtectionMode=0;
		GPIO_ResetBits(GPIOC,GPIO_Pin_10);
		me.isRun=1;
	}
if(mode==0&&me.isRun==1)
	{
		batholithPWMDisable();
		MotorEngineReset();
		batholithPWMDisable();
		brushLessDisable();
		RC_Reset();
		setXSpeed=0;
		setYSpeed=0;
		yunTaiProtectionMode=1;
		//CAN_ITConfig(CAN1,CAN_IT_FMP0,DISABLE);
    GPIO_SetBits(GPIOC,GPIO_Pin_10);
		me.isRun=0;
	}
}
