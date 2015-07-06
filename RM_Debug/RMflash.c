#include "RMflash.h"
/**
*store current parameter settings into flash
*	@arg Yuntai_lp_kp_Address: the Address where lp_kp is stored
* @arg Yuntai_lp_ki_Address: the Address where lp_ki is stored
* @arg Yuntai_lp_kd_Address: the Address where lp_kd is stored
* @arg Yuntai_ly_kd_Address: the Address where ly_kd is stored
* @arg Yuntai_ly_kd_Address: the Address where ly_kd is stored
* @arg Yuntai_ly_kd_Address: the Address where ly_kd is stored
* @arg the value of the parameter to be stored
*/
FLASH_Status storeYunTaiParameters(uint32_t Yuntai_Parameters_Address,double parameters){
	return FLASH_ProgramWord(Yuntai_Parameters_Address,*((uint64_t*)(&parameters)));

}

/**
*store current Key settings into flash
* @arg Forward_Key_value_Address: the Address where the forward key to be stored
* @arg Backward_Key_value_Address: the Address where the backward key to be stored
* @arg Leftward_Key_value_Address: the Address where the leftward key to be stored
* @arg Rightward_Key_value_Address: the Address where the rightward key to be stored
* @arg LeftTurn_Key_value_Address: the Address where the leftturn key to be stored
* @arg rightTurn_Key_value_Address: the Address where the rightturn key to be stored
* @arg KeyValue: the Key Position settings to be stored
*/
FLASH_Status storeUserKeys(uint32_t Key_value_Address, uint16_t KeyValue){
 
	return FLASH_ProgramHalfWord(Key_value_Address,KeyValue);
}
/**
*read parameters of Yuntai from flash
*	@arg Yuntai_lp_kp_Address: the Address where lp_kp is stored
* @arg Yuntai_lp_ki_Address: the Address where lp_ki is stored
* @arg Yuntai_lp_kd_Address: the Address where lp_kd is stored
* @arg Yuntai_ly_kd_Address: the Address where ly_kd is stored
* @arg Yuntai_ly_kd_Address: the Address where ly_kd is stored
* @arg Yuntai_ly_kd_Address: the Address where ly_kd is stored
* @para parameter: the pointer to the parameter to be changed
*/
void readYunTaiParameters(uint32_t Yuntai_Parameters_Address, double* parameter){
 *parameter=*(double *)Yuntai_Parameters_Address;
}
/**
*read parameters of Yuntai from flash
* @arg Forward_Key_value_Address: the Address where the forward key to be stored
* @arg Backward_Key_value_Address: the Address where the backward key to be stored
* @arg Leftward_Key_value_Address: the Address where the leftward key to be stored
* @arg Rightward_Key_value_Address: the Address where the rightward key to be stored
* @arg LeftTurn_Key_value_Address: the Address where the leftturn key to be stored
* @arg rightTurn_Key_value_Address: the Address where the rightturn key to be stored
* @para parameter: the pointer to the parameter to be changed
*/
void readKeyValueSettings(uint32_t Key_Value_Address, uint16_t* keyValue){
 *keyValue=*(uint16_t *)Key_Value_Address;
}

