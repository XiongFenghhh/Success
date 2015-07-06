#ifndef RMFLASH_H
#define RMFLASH_H
#include "RMVariables.h"
//use sector23 to store all Yuntai data
#define Yuntai_lp_kp_Address 	0x08D80000
#define Yuntai_lp_ki_Address 	0x08D80008
#define Yuntai_lp_kd_Address 	0x08D80010
#define Yuntai_ly_kp_Address 	0x08D80018
#define Yuntai_ly_ki_Address 	0x08D80020
#define Yuntai_ly_kd_Address 	0x08D80028

//use sector22 to store all Key setting data
#define Forward_Key_value_Address 	0x08D00030
#define Backward_Key_value_Address 	0x08D00032
#define LeftWard_Key_value_Address 	0x08D00034
#define RightWard_Key_value_Address 0x08D00036
#define LeftTurn_Key_value_Address 	0x08D00038
#define RightTurn_Key_value_Address 0x08D0003A

FLASH_Status storeYunTaiParameters(uint32_t Yuntai_Parameters_Address,double parameters);
FLASH_Status storeUserKeys(uint32_t Key_value_Address, uint16_t KeyValue);
void readYunTaiParameters(uint32_t Yuntai_Parameters_Address, double*);
void readKeyValueSettings(uint32_t Key_Value_Address, uint16_t*);


#endif
