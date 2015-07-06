#ifndef RMVariables_h
#define RMVariables_h

#include "stm32f4xx.h"

#include "MotorEngine.h"
#include "BatholithTimer.h"
#include "RemoteController.h"
#include "Encoder.h"
#include "brushlessMotor.h"
#include "math.h"
#include "RMVhiecleSize.h"
#include "usart3.h"

#include "can.h"
#include "stepper.h"
#include "stdio.h"
#include "mpu6050_i2c.h"
#include "mpu6050_driver.h"
#include "mpu6050_process.h"
#include "mpu6050_interrupt.h"
#include "RMflash.h"

//@modified by huangmin on 2015.06.21
#include "inv_mpu_dmp_motion_driver.h"
#include "inv_mpu.h"
#include "STM32_I2C.h" 
#include "AHRS_Attitude.h"
#include "mpu9150.h"
//end

#define	BM_PWM_MAX	4000
//declaration of struct:MotorEngine and RC_Ctl_t
struct MotorEngine;
struct RC_Ctl_t;
struct __MPU6050_RAW_Data__;
struct __MPU6050_REAL_Data__;
typedef struct __MPU6050_RAW_Data__  MPU6050_RAW_DATA;
typedef struct __MPU6050_REAL_Data__  MPU6050_REAL_DATA;

//declaration of some global variables
extern struct MotorEngine me;
extern  MPU6050_RAW_DATA    MPU6050_Raw_Data; 
extern  MPU6050_REAL_DATA   MPU6050_Real_Data;
extern uint8_t isSpeedUp;

extern int16_t shootStopCount;
extern double setXSpeed;
extern double realXSpeed;
extern double setYSpeed;
extern double realYSpeed;
extern double realWSpeed;
extern double preX;
extern double preY;
extern uint8_t yunTaiProtectionMode;
extern double Kp;
extern double Ki;
extern double Kd;

extern struct RC_Ctl_t RC_Ctl;
extern unsigned char pWord[10];
extern unsigned char vscope_en;

extern int16_t encoder_cnt[4];
 
void delay_us(u32 n);
void delay_ms(uint16_t);
/*
*Initialization of the whole project except MPU9150
*/
void RM_Init(void);
void RM_InterruptInit(void);
void RM_SystemSwitch(uint8_t);

#endif
