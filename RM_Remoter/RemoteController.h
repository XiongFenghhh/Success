#ifndef RC_H
#define RC_H
#include "stm32f4xx.h"

#include "math.h"
#include "RMVariables.h"

/* ----------------------- RC Channel Definition---------------------------- */
#define RC_CH_VALUE_MIN ((uint16_t)364 )
#define RC_CH_VALUE_OFFSET ((uint16_t)1024)
#define RC_CH_VALUE_MAX ((uint16_t)1684)

/* ----------------------- RC Switch Definition----------------------------- */
#define RC_SW_UP ((uint16_t)1)
#define RC_SW_MID ((uint16_t)3)
#define RC_SW_DOWN ((uint16_t)2)

/* ----------------------- PC Key Definition-------------------------------- */
#define KEY_PRESSED_OFFSET_W ((uint16_t)0x01<<0)
#define KEY_PRESSED_OFFSET_S ((uint16_t)0x01<<1)
#define KEY_PRESSED_OFFSET_A ((uint16_t)0x01<<2)
#define KEY_PRESSED_OFFSET_D ((uint16_t)0x01<<3)
#define KEY_PRESSED_OFFSET_Q ((uint16_t)0x01<<4)
#define KEY_PRESSED_OFFSET_E ((uint16_t)0x01<<5)
#define KEY_PRESSED_OFFSET_SHIFT ((uint16_t)0x01<<6)
#define KEY_PRESSED_OFFSET_CTRL ((uint16_t)0x01<<7)

/* ----------------------- Data Struct ------------------------------------- */

struct RC_Ctl_t
{
		struct
		{
				uint16_t ch0;//��ҡ������
				uint16_t ch1;//��ҡ������
				uint16_t ch2;//��ҡ������
				uint16_t ch3;//��ҡ������
				uint8_t s1;	//��ǰ��ť
				uint8_t s2; //��ǰ��ť
		}rc;
		struct
		{
				int16_t x;//���x��
				int16_t y;//���y��
				int16_t z;
				uint8_t press_l; //������
				uint8_t press_r;// ����Ҽ�
				uint8_t isDownedge_r;
				uint8_t isUpedge_r;
				uint8_t isClicked_l;
				uint8_t isClicked_r;
		}mouse;
		struct
		{
				uint16_t v; //�ܼ�ֵ
				uint16_t forward;
				uint16_t backward;
				uint16_t leftward;
				uint16_t rightward;
				uint16_t shift;
				uint16_t ctrl;
				uint16_t leftTurn;
				uint16_t rightTurn;
//			ÿ��������Ӧһ��bit��������ʾ
//			Bit0 �\�\�\�\�\�\�\ W ��
//			Bit1 �\�\�\�\�\�\�\ S ��
//			Bit2 �\�\�\�\�\�\�\ A ��
//			Bit3 �\�\�\�\�\�\�\ D ��
//			Bit4 �\�\�\�\�\�\�\ Shift ��
//			Bit5 �\�\�\�\�\�\�\ Ctrl ��
//			Bit6 �\�\�\�\�\�\�\ Q ��
//			Bit7 �\�\�\�\�\�\�\ E ��
		}key;
		struct
		{
			uint16_t forward;
			uint16_t backward;
			uint16_t leftward;
			uint16_t rightward;
			uint16_t leftturn;
			uint16_t  rightturn;
		}setKey;
		/*@author xiong
		*@date 2015.4.15
		*description:
		velocity indicates the desired velocity of the vehicle
		y
		^
		|
		|
		|-------->x
		
		*/
		struct{
		double vel;
		double x;
		double y;
		double w;
		uint32_t BMPWM;
		uint8_t isBMSet;
		uint8_t isStepperMoving;
			#ifdef Gun
			int steerPWM;
			#endif
		}velocity;
};

/* ----------------------- Internal Data ----------------------------------- */



/* ----------------------- Function Declarations---------------------------- */
//void RC_Ctl_Init();
//void RC_Ctl_Reset();


void RC_Reset(void);
void RC_Init(void);
void RC_Receive(void);
void RC_Convert(void);
//void RC_Ctl_Pconvert();

double getXvelocity(void);
double getYvelocity(void);
double getWvelocity(void);
uint32_t getBMPWM(void);
double getYunTaiDeltaPositionPitch(void);
double getYunTaiDeltaPositionYaw(void);
uint16_t isYunTaiYawMoving(void);
uint16_t isAutoTarget(void);
uint8_t isStepperMoving(void);
/**
*for debugging purpose
* @return FLASH_COMPLETE: storage success;
* 				FLASH_PROGRAM_ERRROR: storage failure;
*/
FLASH_Status storeCurrentKeySettings(void);
void loadKeySettings(void);
void getMouse(void);
#endif
 
