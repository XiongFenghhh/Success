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
				uint16_t ch0;//ÓÒÒ¡¸Ë×óÓÒ
				uint16_t ch1;//ÓÒÒ¡¸ËÉÏÏÂ
				uint16_t ch2;//×óÒ¡¸Ë×óÓÒ
				uint16_t ch3;//×óÒ¡¸ËÉÏÏÂ
				uint8_t s1;	//×óÇ°²¦Å¥
				uint8_t s2; //ÓÒÇ°²¦Å¥
		}rc;
		struct
		{
				int16_t x;//Êó±êxÖá
				int16_t y;//Êó±êyÖá
				int16_t z;
				uint8_t press_l; //Êó±ê×ó¼ü
				uint8_t press_r;// Êó±êÓÒ¼ü
				uint8_t isDownedge_r;
				uint8_t isUpedge_r;
				uint8_t isClicked_l;
				uint8_t isClicked_r;
		}mouse;
		struct
		{
				uint16_t v; //×Ü¼üÖµ
				uint16_t forward;
				uint16_t backward;
				uint16_t leftward;
				uint16_t rightward;
				uint16_t shift;
				uint16_t ctrl;
				uint16_t leftTurn;
				uint16_t rightTurn;
//			Ã¿¸ö°´¼ü¶ÔÓ¦Ò»¸öbit£¬ÈçÏÂËùÊ¾
//			Bit0 ©\©\©\©\©\©\©\ W ¼ü
//			Bit1 ©\©\©\©\©\©\©\ S ¼ü
//			Bit2 ©\©\©\©\©\©\©\ A ¼ü
//			Bit3 ©\©\©\©\©\©\©\ D ¼ü
//			Bit4 ©\©\©\©\©\©\©\ Shift ¼ü
//			Bit5 ©\©\©\©\©\©\©\ Ctrl ¼ü
//			Bit6 ©\©\©\©\©\©\©\ Q ¼ü
//			Bit7 ©\©\©\©\©\©\©\ E ¼ü
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
 
