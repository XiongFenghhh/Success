/**************************************
 File name: VelocityRotationPWM.h
 Author:ÐÜ·å Date:2015-5-1
 Description: MotorEngine contains related information about motors
**************************************/
#ifndef VelocityRotationPWM_h
#define VelocityRotationPWM_h

//#include "Velocity.h"


#include "RMVariables.h"



#define abs(x) ((x)>0? (x):(-(x)))

struct MotorEngine{
	uint8_t isRun;
	
	double rotation[4];
	double rotation_fil[4];
	
	double errors[4];
	double preErrors[4];
	double preErrors2[4];
	double maxAbs;
	

  double pwm[4];
	uint8_t isPIDAllowed;
	
	int isStart;
	uint8_t isPWMallowed;
	double xRatio;
  double yRatio;
  double wRatio;
};

void MotorEngineReset(void);
void MotorEngine_Config(void);
void CalcRotations(void);

void PIDAlgorithm(void);

#endif
