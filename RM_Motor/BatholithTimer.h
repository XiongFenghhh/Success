#ifndef BatholithTimer_h
#define BatholithTimer_h
#include "RMVariables.h"
//config Timer8 Timer1
void RmBatholicTIM_PWM_Config(void);
void RmBatholicTIM8_Config(void);
void RmBatholicTIM1_Config(void);

//set pwm ratio
void batholithSetPwm(double pwm,GPIO_TypeDef* GPIOx, uint16_t GPIO_PinSource);
void batholithResetPwm(void);
void setDeadTime(uint8_t);
uint8_t getDeadTime(void);
void setPiancha1(int);
void setPiancha2(int);
void setPiancha3(int);
void setPiancha4(int);

//enable/disable timer8,timer1
void batholithPWMEnable(void);
void batholithPWMDisable(void);
#endif
