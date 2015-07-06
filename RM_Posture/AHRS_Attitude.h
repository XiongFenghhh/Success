#ifndef __AHRS_Attitude_H
#define __AHRS_Attitude_H

#include "stm32f4xx.h"

#define MPU9150_Addr            0x68
#define P_M_1                   0x6B
#define Accel_FILTER_98HZ		98
#define Accel_Xout_H		    0x3B
#define Gyro_Xout_H		        0x43
#define Bypass_Enable_Cfg		0x37
#define User_Ctrl               0x6A
#define Compass_Addr            0x0C
#define Compass_CNTL            0x0A
#define Compass_ST1             0x02
#define	Compass_HXL             0x03

extern float Pitch, Roll, Yaw;
extern float halfT;
extern float init_ax, init_ay, init_az, init_gx, init_gy, init_gz, init_mx, init_my, init_mz;

extern float MXgain;
extern float MYgain;
extern float MZgain;
extern float MXoffset;
extern float MYoffset;
extern float MZoffset;
extern float heading;

void get_mpu9150_data(void);
void init_quaternion(void);
void AHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);
float invSqrt(float x);
void Get_Attitude(void);
int get_gyro_bias(void);
void get_compass_bias(void);
void compass_calibration(void);
void Read_MPU9150_Mag(void);
int Init_MPU9150_Mag(void);

#endif
