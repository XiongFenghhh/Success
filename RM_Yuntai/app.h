#ifndef __APP_H__
#define __APP_H__
#include "RMVariables.h"

#define ESC_MAX 5000.0
#define abs(x) ((x)>0? (x):(-(x)))

void Cmd_ESC(int16_t current_201,int16_t current_202,int16_t current_203);

double Velocity_Control_201(double current_velocity_201,double target_velocity_201);
double Position_Control_201(double current_position_201,double target_position_201);
double Velocity_Control_201_Shoot(double current_velocity_201,double target_velocity_201);

double Velocity_Control_203(double current_velocity_203,double target_velocity_203);
double Position_Control_203(double current_position_203,double target_position_203);
//@modified by huangmin on 2015.04.25
double Current_Control_203(double current_203,double target_current_203);
double followControl(int current_position_203);
void outputData(void);
void setPitchPositionParameters(double kp,double ki,double kd);
void setYawPositionParameters(double kp,double ki,double kd);

/**
* @return: FLASH_COMPLETE: storage success
* 				 FLASH_PROGRAM_ERROR: storage failure
*/
FLASH_Status storeCurrentYuntaiParameters(void);
void loadYuntaiParameters(void);
#endif
