#include "RemoteController.h"

static unsigned char sbus_rx_buffer[18];

static double preMouseY;
static double preMouseX;
static uint32_t mousePressedTime=0;
static double rotationRatio=1;

void cumulateMousePressedTime(void)
{
	if(mousePressedTime<700&&RC_Ctl.mouse.press_r==1)mousePressedTime++;
}


void RC_Reset(void)
{
		RC_Ctl.velocity.isStepperMoving=0;
		RC_Ctl.velocity.BMPWM=0.05*BM_PWM_MAX;
		RC_Ctl.velocity.isBMSet=0;
	#ifndef Gun
		RC_Ctl.velocity.vel=1550;
	#endif
	#ifdef Gun
		RC_Ctl.velocity.steerPWM=332;
		RC_Ctl.velocity.vel=1050;
	#endif
		RC_Ctl.rc.ch3=1024;
		RC_Ctl.mouse.x=0;
		RC_Ctl.mouse.y=0;
		RC_Ctl.key.forward=0;
		RC_Ctl.key.backward=0;
		RC_Ctl.key.leftTurn=0;
		RC_Ctl.key.rightTurn=0;
		RC_Ctl.key.leftward=0;
		RC_Ctl.key.rightward=0;
	setXSpeed=0;
	realXSpeed=0;
	realYSpeed=0;
	setYSpeed=0;
	realWSpeed=0;
}

/**
*description: Remote Controller Initialization.including gpio,dma
*@para none
*@retVal none
*called by: RM_Init()
*/
void RC_Init(void)
{
/* -------------- Enable Module Clock Source ----------------------------*/
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_DMA1, ENABLE);
RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	
/* -------------- Configure GPIO ---------------------------------------*/
	{
		GPIO_InitTypeDef gpio;
				USART_InitTypeDef usart2;
				gpio.GPIO_Pin = GPIO_Pin_3;
				gpio.GPIO_Mode = GPIO_Mode_AF;
				gpio.GPIO_OType = GPIO_OType_PP;
				gpio.GPIO_Speed = GPIO_Speed_100MHz;
				gpio.GPIO_PuPd = GPIO_PuPd_UP;
				GPIO_Init(GPIOA, &gpio);
GPIO_PinAFConfig(GPIOA,GPIO_PinSource3, GPIO_AF_USART2);
USART_DeInit(USART2);
				usart2.USART_BaudRate = 100000;
				usart2.USART_WordLength = USART_WordLength_8b;
				usart2.USART_StopBits = USART_StopBits_1;
				usart2.USART_Parity = USART_Parity_Even;
				usart2.USART_Mode = USART_Mode_Rx;
				usart2.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
				USART_Init(USART2,&usart2);
				USART_Cmd(USART2,ENABLE);
				USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);
	}
		
		/* -------------- Configure DMA -----------------------------------------*/
		{
				DMA_InitTypeDef dma;
				DMA_DeInit(DMA1_Stream5);
				dma.DMA_Channel = DMA_Channel_4;
				dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART2->DR);
				dma.DMA_Memory0BaseAddr = (uint32_t)sbus_rx_buffer;
				dma.DMA_DIR = DMA_DIR_PeripheralToMemory;
				dma.DMA_BufferSize = 18;
				dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
				dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
				dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
				dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
				dma.DMA_Mode = DMA_Mode_Circular;
				dma.DMA_Priority = DMA_Priority_VeryHigh;
				dma.DMA_FIFOMode = DMA_FIFOMode_Disable;
				dma.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
				dma.DMA_MemoryBurst = DMA_Mode_Normal;
				dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
				DMA_Init(DMA1_Stream5,&dma);
				DMA_ITConfig(DMA1_Stream5,DMA_IT_TC,DISABLE);
				DMA_Cmd(DMA1_Stream5,ENABLE);
		}
		RC_Ctl.setKey.forward=0x0001;
		RC_Ctl.setKey.backward=0x0002;
		RC_Ctl.setKey.leftturn=0x0040;
		RC_Ctl.setKey.rightturn=0x0080;
		RC_Ctl.setKey.leftward=0x0004;
		RC_Ctl.setKey.rightward=0x0008;
		RC_Reset();
}


/**isButtonPressed
*@description judge whether a button is pressed by up edge
*@para button: the previous value of the specific button
*@para currentValue: the current value of the specific button
*@retVal 1---the button is pressed
				 0---the button isn't pressed
*/
uint8_t isButtonPressed(uint16_t button,  uint16_t currentValue)
{
	if(button==0&&currentValue!=0)return 1;
	else return 0;
}


/**
*description: 1.Receive signals generated by remote controller
*							2.Assign each key
*@retVal none
*@para none
*called by:main()
*/
void RC_Receive(void){
				RC_Ctl.rc.ch0 = (sbus_rx_buffer[0]| (sbus_rx_buffer[1] << 8)) & 0x07ff; //!< Channel 0
				RC_Ctl.rc.ch1 = ((sbus_rx_buffer[1] >> 3) | (sbus_rx_buffer[2] << 5)) & 0x07ff; //!< Channel 1
				RC_Ctl.rc.ch2 = ((sbus_rx_buffer[2] >> 6) | (sbus_rx_buffer[3] << 2) | //!< Channel 2
				(sbus_rx_buffer[4] << 10)) & 0x07ff;
				RC_Ctl.rc.ch3 = ((sbus_rx_buffer[4] >> 1) | (sbus_rx_buffer[5] << 7)) & 0x07ff; //!< Channel 3
				
				
	
				preMouseY=RC_Ctl.mouse.y-(double)(RC_Ctl.rc.ch3 -1024)/3;
				preMouseX=RC_Ctl.mouse.x;
				RC_Ctl.mouse.x = sbus_rx_buffer[6] | (sbus_rx_buffer[7] << 8); //!< Mouse X axis
				RC_Ctl.mouse.y = sbus_rx_buffer[8] | (sbus_rx_buffer[9] << 8); //!< Mouse Y axis
				RC_Ctl.mouse.z = sbus_rx_buffer[10] | (sbus_rx_buffer[11] << 8); //!< Mouse Z axis
	
				
				RC_Ctl.mouse.press_l = sbus_rx_buffer[12]; //!< Mouse Left Is Press ?
				
				if((sbus_rx_buffer[13]==1&&RC_Ctl.mouse.press_r==0)||((((sbus_rx_buffer[5] >> 4)& 0x000C) >> 2)==1&&RC_Ctl.rc.s2==3))
				{
					RC_Ctl.mouse.isUpedge_r=1;
					RC_Ctl.mouse.isDownedge_r=0;
				}
				else if((sbus_rx_buffer[13]==1&&RC_Ctl.mouse.press_r==1)||((((sbus_rx_buffer[5] >> 4)& 0x000C) >> 2)==1&&RC_Ctl.rc.s2==1))
				{
					RC_Ctl.mouse.isUpedge_r=1;
					RC_Ctl.mouse.isDownedge_r=0;
				}else if((sbus_rx_buffer[13]==0&&RC_Ctl.mouse.press_r==1)||((((sbus_rx_buffer[5] >> 4)& 0x000C) >> 2)==3&&RC_Ctl.rc.s2==1))
				{
					RC_Ctl.mouse.isUpedge_r=1;
					RC_Ctl.mouse.isDownedge_r=1;
				}else
				{
					RC_Ctl.mouse.isUpedge_r=0;
					RC_Ctl.mouse.isDownedge_r=0;
				}
				if(RC_Ctl.mouse.isDownedge_r==1&&RC_Ctl.mouse.isUpedge_r==1)RC_Ctl.mouse.isClicked_r=1;
				else RC_Ctl.mouse.isClicked_r=0;
				RC_Ctl.mouse.press_r = sbus_rx_buffer[13]; //!< Mouse Right Is Press ?
				RC_Ctl.rc.s2 = ((sbus_rx_buffer[5] >> 4)& 0x000C) >> 2; //!< Switch Left
				RC_Ctl.rc.s1 = ((sbus_rx_buffer[5] >> 4)& 0x0003); //!< Switch Right
				RC_Ctl.key.v = sbus_rx_buffer[14] | (sbus_rx_buffer[15] << 8); //!< KeyBoard value
				RC_Ctl.key.forward = (RC_Ctl.key.v & RC_Ctl.setKey.forward)==0?0:1;
				//2015.6.24 slow mode
				//if(RC_Ctl.key.forward==0)yRatio=0;
					
				
				RC_Ctl.key.backward = (RC_Ctl.key.v & RC_Ctl.setKey.backward)==0?0:1;
				RC_Ctl.key.leftward = (RC_Ctl.key.v & RC_Ctl.setKey.leftward)==0?0:1;
				RC_Ctl.key.rightward = (RC_Ctl.key.v & RC_Ctl.setKey.rightward)==0?0:1;
				if(isButtonPressed(RC_Ctl.key.shift,(RC_Ctl.key.v & 0x0010))==1)
				{
					#ifndef Gun
					RC_Ctl.velocity.vel=1650-RC_Ctl.velocity.vel;
					rotationRatio=1.5-rotationRatio;
					#endif
					#ifdef Gun
					RC_Ctl.velocity.vel=1450-RC_Ctl.velocity.vel;
					rotationRatio=1.5-rotationRatio;
					#endif
				}
				RC_Ctl.key.shift = (RC_Ctl.key.v & 0x0010)==0?0:1;
#ifdef Gun
				if(RC_Ctl.rc.s1==1)
				{
					if(isButtonPressed(RC_Ctl.key.ctrl,RC_Ctl.key.v & 0x0020))
					RC_Ctl.velocity.steerPWM=653-	RC_Ctl.velocity.steerPWM;
				}
#endif
				RC_Ctl.key.ctrl = (RC_Ctl.key.v & 0x0020)==0?0:1;
				RC_Ctl.key.leftTurn = (RC_Ctl.key.v & RC_Ctl.setKey.leftturn)==0?0:1;
				RC_Ctl.key.rightTurn = (RC_Ctl.key.v & RC_Ctl.setKey.rightturn)==0?0:1;
				RC_Ctl.key.shift = (RC_Ctl.key.v & 0x0010)==0?0:1;
				//RC_Ctl.velocity.vel=RC_Ctl.key.shift==1?400:1550;	
			
}



/**
*Convert the signals into motion command of motor and yuntai
*/


void RC_Convert(void){
double tempYSpeed=0;
double tempXSpeed=0;
double tempWSpeed=0;
/**=====================================Shooting part==================================**/
if(me.isStart==1)
{
	
if(RC_Ctl.mouse.isUpedge_r==1&&RC_Ctl.velocity.isBMSet==0&&RC_Ctl.velocity.isStepperMoving==0)
		{
			if(RC_Ctl.velocity.BMPWM<0.1*BM_PWM_MAX-50)
			{
				RC_Ctl.velocity.BMPWM=BM_PWM_MAX*0.1-50;
				GPIO_ResetBits(GPIOB,GPIO_Pin_6);
				RC_Ctl.velocity.isBMSet=1;
			}
			else if(shootStopCount>350)
			{
				RC_Ctl.velocity.BMPWM=BM_PWM_MAX*0.05;
				GPIO_SetBits(GPIOB,GPIO_Pin_6);
				RC_Ctl.velocity.isBMSet=1;
			}
			
		}
	else if(RC_Ctl.mouse.isUpedge_r==0)
		{
			RC_Ctl.velocity.isBMSet=0;
			shootStopCount=0;
		}
	if(RC_Ctl.mouse.press_l==1||RC_Ctl.rc.s2==2)
		{
			RC_Ctl.velocity.isStepperMoving=1;
		}else
		{
			RC_Ctl.velocity.isStepperMoving=0;
		}
/**================================Debugging Part=======================================**/
	
}	
/**=================================Remote Controller Part===========================**/
	if(RC_Ctl.rc.s1==3)
	{
			me.isStart=1;
	if((RC_Ctl.rc.ch0-1024)*(RC_Ctl.rc.ch0-1024)+(RC_Ctl.rc.ch1-1024)*(RC_Ctl.rc.ch1-1024)>680*680)
	{
		setXSpeed=((double)(RC_Ctl.rc.ch0-1024)/sqrtf((RC_Ctl.rc.ch0-1024)*(RC_Ctl.rc.ch0-1024)+(RC_Ctl.rc.ch1-1024)*(RC_Ctl.rc.ch1-1024))*1200);
		setYSpeed=((double)(RC_Ctl.rc.ch1-1024)/sqrtf((RC_Ctl.rc.ch0-1024)*(RC_Ctl.rc.ch0-1024)+(RC_Ctl.rc.ch1-1024)*(RC_Ctl.rc.ch1-1024))*1200);
	}else
	{
		setXSpeed=((double)(RC_Ctl.rc.ch0-1024)/660*1200);
		setYSpeed=((double)(RC_Ctl.rc.ch1-1024)/660*1200);
	}
		RC_Ctl.velocity.w=(double)(RC_Ctl.rc.ch2-1024)/660*0.5;
	
	}
/**=====================================Key Board Part=============================**/
	else if(RC_Ctl.rc.s1==1)
	{
		me.isStart=1;	
			RC_Ctl.velocity.w=0;
			/**==========================Forward and BackWard Control=======================**/	
			
					if(RC_Ctl.key.forward==1&&RC_Ctl.key.backward==0)
						{
							tempYSpeed=RC_Ctl.velocity.vel;
						}
					else if(RC_Ctl.key.backward==1&&RC_Ctl.key.forward==0)
						{
							tempYSpeed=-RC_Ctl.velocity.vel;
						}
					else 
							tempYSpeed=0;	
					if(tempYSpeed!=setYSpeed)
							preY=setYSpeed;
					else 
							preY=preY;
					
					setYSpeed=tempYSpeed;
			
			
			/**=====================Left and Right Control===============================**/
				
			
				if(RC_Ctl.key.leftward==1&&RC_Ctl.key.rightward==0)
					{
						tempXSpeed=-RC_Ctl.velocity.vel*0.6;
					}
				else if(RC_Ctl.key.rightward==1&&RC_Ctl.key.leftward==0)
					{
						tempXSpeed=RC_Ctl.velocity.vel*0.6;
					}
				else 
						tempXSpeed=0;
				if(tempXSpeed!=setXSpeed)
						preX=setXSpeed;
				else 
						preX=preX;
						
				setXSpeed=tempXSpeed;
			
			/**==========================Rotation Control=================================**/
				if(RC_Ctl.key.leftTurn==1&&RC_Ctl.key.rightTurn==0)
				{
					tempWSpeed=-0.05;
				}
				else if(RC_Ctl.key.rightTurn==1&&RC_Ctl.key.leftTurn==0)
				{
					tempWSpeed=0.05;
				}else
				{	
					tempWSpeed=0;
					if(RC_Ctl.mouse.x!=0)
						{
							if(RC_Ctl.mouse.x>99)tempWSpeed=0.15;
							else if(RC_Ctl.mouse.x<-99)tempWSpeed=-0.15;
							else tempWSpeed=((double)(RC_Ctl.mouse.x))/70;
						}
				}
			RC_Ctl.velocity.w=tempWSpeed;
				
			
	}else me.isStart=0;
/**====================================Vision Part======================================**/
#ifndef Gun
	setIsAutoTargetMode(RC_Ctl.key.ctrl);
	#endif
/**====================================Yuntai Part======================================**/	
setYunTaiPosition(getYunTaiDeltaPositionPitch(),0);
}

/**
*@description return the velocity of vheicle
*@para none
*@retVal realYSpeed/realXSpeed/realWSpeed: the vheicle velocity x,y,w;
*/
double getYvelocity(void){return realYSpeed;}
double getXvelocity(void){return realXSpeed;}
double getWvelocity(void){return realWSpeed;}




/**
*@description get the pwm for steering moto and brushless moto
*@para none
*@retVal the pwm for brushless moto 
*/
uint32_t getBMPWM(void)
{
#ifdef Gun
if(RC_Ctl.rc.s1==3)
RC_Ctl.velocity.steerPWM=332+((double)(RC_Ctl.rc.ch3-1024)/660*200);

//	RC_Ctl.velocity.steerPWM+=(double)(RC_Ctl.mouse.y)/15;
if(RC_Ctl.velocity.steerPWM<320)RC_Ctl.velocity.steerPWM=315;
else if(RC_Ctl.velocity.steerPWM>332)RC_Ctl.velocity.steerPWM=338 ;
	TIM_SetCompare2(TIM9,RC_Ctl.velocity.steerPWM);
#endif	
return RC_Ctl.velocity.BMPWM;
}
uint8_t isStepperMoving(void){return RC_Ctl.velocity.isStepperMoving;}
void getMouse(void)
{
	printf("%d  %d\r\n",RC_Ctl.mouse.x,RC_Ctl.mouse.y);
}


//������
double getYunTaiDeltaPositionYaw(void){
	if(RC_Ctl.rc.s1!=2){
		return RC_Ctl.velocity.w;
	}else return 0;

}

double getYunTaiDeltaPositionPitch(void){
	
	if(RC_Ctl.rc.s1==1){
		if(RC_Ctl.mouse.y>80)return 1;
		else if(RC_Ctl.mouse.y<-80)return -1;
		else return (double)(RC_Ctl.mouse.y)/80;
	}else if(RC_Ctl.rc.s1==3){
		return (-(double)(RC_Ctl.rc.ch3-1024)/3)/150;
	}else return 0;
}
uint16_t isYunTaiYawMoving(void){
if(RC_Ctl.mouse.y>1||RC_Ctl.mouse.y<-1)
return 1;
else 
return 0;
}


//called by:main()
uint16_t isAutoTarget(void){
if(RC_Ctl.key.ctrl==1)return 1;
	else return 0;
}





FLASH_Status storeCurrentKeySettings(void){
FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGPERR | FLASH_FLAG_WRPERR);
	 if (FLASH_EraseSector(FLASH_Sector_22 , VoltageRange_3) != FLASH_COMPLETE)
    { 
      return FLASH_ERROR_PROGRAM;
    }
	if(storeUserKeys(Forward_Key_value_Address,RC_Ctl.setKey.forward)!=FLASH_COMPLETE){
	return FLASH_ERROR_PROGRAM;
	}
	if(storeUserKeys(Backward_Key_value_Address,RC_Ctl.setKey.backward)!=FLASH_COMPLETE){
	return FLASH_ERROR_PROGRAM;
	}
	if(storeUserKeys(LeftWard_Key_value_Address,RC_Ctl.setKey.leftward)!=FLASH_COMPLETE){
	return FLASH_ERROR_PROGRAM;
	}
	if(storeUserKeys(RightWard_Key_value_Address,RC_Ctl.setKey.rightward)!=FLASH_COMPLETE){
	return FLASH_ERROR_PROGRAM;
	}
	if(storeUserKeys(LeftTurn_Key_value_Address,RC_Ctl.setKey.leftturn)!=FLASH_COMPLETE){
	return FLASH_ERROR_PROGRAM;
	}
	if(storeUserKeys(RightTurn_Key_value_Address,RC_Ctl.setKey.rightturn)!=FLASH_COMPLETE){
	return FLASH_ERROR_PROGRAM;
	}
	FLASH_Lock();	
	return FLASH_COMPLETE;
}

void loadKeySettings(void){
readKeyValueSettings(Forward_Key_value_Address,&(RC_Ctl.setKey.forward));
	readKeyValueSettings(Backward_Key_value_Address,&(RC_Ctl.setKey.backward));
	readKeyValueSettings(LeftWard_Key_value_Address,&(RC_Ctl.setKey.leftward));
	readKeyValueSettings(RightWard_Key_value_Address,&(RC_Ctl.setKey.rightward));
	readKeyValueSettings(LeftTurn_Key_value_Address,&(RC_Ctl.setKey.leftturn));
	readKeyValueSettings(RightTurn_Key_value_Address,&(RC_Ctl.setKey.leftturn));
}