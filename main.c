#include "main.h"

/**
*before the main() was excuted, the RCC and Flash are already configured in SystemInit();
*if you want to re-configure RCC and Flash goto SetSysClock();
*/
int main(){
int i=0; 
	RM_Init();
	delay_ms(500);
	i2cInit(); 
  init_mpu9150();	 
	
	   init_quaternion();        //得到初始化四元数
   RM_InterruptInit();	
	while(1){
		RC_Receive();
		RC_Convert();
 //  printf("%f %f %d %d\r\n",me.pwm[0],me.pwm[1],encoder_cnt[2],encoder_cnt[3]);
//		PIDAlgorithm();
	if(me.isStart)
			{
			RM_SystemSwitch(1);			
			PIDAlgorithm();
			BMotor_PWM(1); //right
			BMotor_PWM(2); //left


		if(getIsReceive()==1)
			{
				//outputData();	
				resetIsReceive();
				setMotoParameter();
			}


		}else RM_SystemSwitch(0);
	}

return 0;
	
}



