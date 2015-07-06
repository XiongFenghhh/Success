/**
  ******************************************************************************
  * @file    LTDC_Display_2Layers/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"

/** @addtogroup STM32F429I_DISCOVERY_Examples
  * @{
  */

/** @addtogroup LTDC_Display_2Layers
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint8_t itTimes=0;

static uint8_t brushStart=0;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
/**
	*时间片划分：
  *__|__|__|__|__|__|
	*  1  2  1  2  1  2
	*每个 2.5ms:如果有发射命令，则控制摩擦轮与步进电机
	*遥控器计算暂时放在主程序中进行
	*第一2.5ms：计算位姿：
							由于MPU的原因，每隔两个毫秒需要计算一次
							@xf：云台电机自带的数据控制底盘旋转，MPU
									的反馈控制炮管具体朝向。
	*第二2.5ms：给出云台与底盘控制信号
	@TODO
2.5ms的系统中断，主函数中的遥控器读取函数将无法运行
	*/
void SysTick_Handler(void)
{
	 CAN_ITConfig(CAN1, CAN_IT_FMP0, DISABLE); //@modified by huangmin 2015.06.02 
	itTimes=itTimes%8;
	Encoder_Get();
 get_mpu9150_data();		 //读取传感器数据
Yaw += init_gz*0.005*57.29578;

if(me.isRun==1&&me.isStart==1)
{
	if(itTimes%2==0)
	{

		if(RC_Ctl.rc.s1==1)
		{
			if(abs(realXSpeed-setXSpeed)<100)
			{
				realXSpeed=setXSpeed;
			}else
			{
				realXSpeed+=0.08*(setXSpeed-preX);
			}
			
			if(abs(realYSpeed-setYSpeed)<100)
			{
				realYSpeed=setYSpeed;
			}else
			{
				realYSpeed+=0.08*(setYSpeed-preY);
			}

		}
	else if(RC_Ctl.rc.s1==3)
	{
		realXSpeed=setXSpeed;
		realYSpeed=setYSpeed;
	}

	}
	if(itTimes%2==1)
	{	
		/*we might use the first dimension of errors to implement differential controll*/
		me.preErrors[0]=me.errors[0];
		me.preErrors[1]=me.errors[1];
		me.preErrors[2]=me.errors[2];
		me.preErrors[3]=me.errors[3];

		
		me.errors[0]=me.rotation_fil[0]-encoder_cnt[0];
		me.errors[1]=me.rotation_fil[1]+encoder_cnt[1];
		me.errors[2]=me.rotation_fil[2]+encoder_cnt[2];
		me.errors[3]=me.rotation_fil[3]-encoder_cnt[3];
		me.isPIDAllowed=1;
		
	}
}

	/**
	*shooting protection
	*/
//cumulateMousePressedTime();
	if(getBMPWM()>200)
		{

			if(isStepperMoving()==1){//&&brushStart>10){
			#ifndef Gun
				Stepper_Ctrl(itTimes);
			#endif
			#ifdef Gun
				Stepper_Ctrl(itTimes);
			#endif
		}
			if(RC_Ctl.velocity.isStepperMoving!=1&&(RC_Ctl.mouse.press_r==1||RC_Ctl.rc.s2==1))
			{
				if(shootStopCount<400)shootStopCount++;
			}else shootStopCount=0;
		} 
	
	else 
	{
			brushStart=0;
			GPIO_ResetBits(GPIOD,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3);
	}
	
itTimes++;
	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);//@hmodified by huangmin 2015.06.02 
	
}


/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f429_439xx.s).  */
/******************************************************************************/

/******************************************************************************
* @fn DMA1_Stream5_IRQHandler
*
* @brief USART2 DMA ISR
*
* @return None.
*
* @note This code is fully tested on STM32F405RGT6 Platform, You can port it
* to the other platform.
*/

/**
  * @}
  */ 

/**
  * @}
  */ 
	
void DMA1_Stream5_IRQHandler(void)
{
		if(DMA_GetITStatus(DMA1_Stream5, DMA_IT_TCIF5))
		{
				DMA_ClearFlag(DMA1_Stream5, DMA_FLAG_TCIF5);
			
				
			
		}
}
void TIM3_IRQHandler(void)
{  
	

}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
