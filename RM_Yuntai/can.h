#ifndef __CAN_H
#define __CAN_H	
 
#include "stm32f4xx.h"	
#include "app.h" //@modified by huangmin on 2015.04.17
#include "stdio.h"//@modified by huangmin on 2015.04.17	 


//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//CAN���� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/7
//�汾��V1.0 
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 

	 							    
u8 CAN1_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);//CAN��ʼ��
 
u8 CAN1_Send_Msg(u8* msg,u8 len);						//��������

//void Cmd_ESC(int16_t current_201,int16_t current_202,int16_t current_203);

u8 CAN1_Receive_Msg(u8 *buf);							//��������

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

















