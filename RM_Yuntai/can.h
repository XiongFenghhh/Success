#ifndef __CAN_H
#define __CAN_H	
 
#include "stm32f4xx.h"	
#include "app.h" //@modified by huangmin on 2015.04.17
#include "stdio.h"//@modified by huangmin on 2015.04.17	 


//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//CAN驱动 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/7
//版本：V1.0 
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 

	 							    
u8 CAN1_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);//CAN初始化
 
u8 CAN1_Send_Msg(u8* msg,u8 len);						//发送数据

//void Cmd_ESC(int16_t current_201,int16_t current_202,int16_t current_203);

u8 CAN1_Receive_Msg(u8 *buf);							//接收数据

void CAN1_RX0_IRQHandler(void);

//added by huangmin on 2015.04.25
//pWord for visual scope display 



//called by RC_Convert()
void setIsAutoTargetMode(uint8_t);
void setYunTaiPosition(double,double);

void storeCurrentYuntai(void);
//judge if object is targeted
//called by main
void setIsYawTargeted(int8_t);
void setIsPitchTargeted(int8_t);
void setWanted_pWanted_y(uint16_t,uint16_t);
//for debugging
void printCurrentPosition(void);
#endif

















