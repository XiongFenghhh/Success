#include "main.h"
static int16_t stepperFoward=0;
static int16_t stepperBack=0;
/**
*before the main() was excuted, the RCC and Flash are already configured in SystemInit();
*if you want to re-configure RCC and Flash goto SetSysClock();
*/
int main(){
int times=0; 
	RM_Init();
	delay_ms(500);
	#ifndef Gun
	i2cInit(); 
  init_mpu9150();	 
	init_quaternion();        //得到初始化四元数
	#endif
   RM_InterruptInit();	
	while(1){
		RC_Receive();
		RC_Convert();
	if(me.isStart)
			{
			RM_SystemSwitch(1);			
			PIDAlgorithm();
			BMotor_PWM(1); //right
			#ifndef Gun
			BMotor_PWM(2); //left
			#endif
			times++;
			times%=1600;
if(getBMPWM()>220&&RC_Ctl.velocity.isStepperMoving==1&&(times%200==0)){
//			if(stepperFoward<400)
//			{
				Stepper_Ctrl(times/200);
//				stepperFoward++;
//			}
//			else if(stepperBack<400)
//			{
//				Stepper_Ctrl(7-(times/200));
//				stepperBack++;
//			}else
//			{
//				stepperFoward=0;
//				stepperBack=0;
//			}
		
		}
	

		
		if(getIsReceive()==1)
			{
				resetIsReceive();
				setMotoParameter();
			}


		}else RM_SystemSwitch(0);
	}

return 0;
	
}



