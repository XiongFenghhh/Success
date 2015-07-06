#include "mpu6050_process.h"

ACCEL_AVERAGE_DATA   Accel_Raw_Average_Data; 
GYRO_RADIAN_DATA     Gyro_Radian_Data;
MPU6050_ANGLE        MPU6050_Angle;

void MPU6050_Data_Filter(void)  // this*0.01 + (上一次用的)last *0.99 要改
{
//        unsigned int i=0;
//        accel_x_buffer[filter_cnt] = MPU6050_Raw_Data.Accel_X;
//        accel_y_buffer[filter_cnt] = MPU6050_Raw_Data.Accel_Y;
//        accel_z_buffer[filter_cnt] = MPU6050_Raw_Data.Accel_Z;   
//        
//        filter_cnt ++;
//        if(filter_cnt == 10)
//        {
//            filter_cnt = 0;
//        }        
//   
//    
//    for(i=0;i<10;i++)
//    {
//        temp_accel_x += accel_x_buffer[i];
//        temp_accel_y += accel_y_buffer[i];
//        temp_accel_z += accel_z_buffer[i];
//    }
//    
//    Accel_Raw_Average_Data.X = (float)temp_accel_x / 10.0;
//    Accel_Raw_Average_Data.Y = (float)temp_accel_y / 10.0;
//    Accel_Raw_Average_Data.Z = (float)temp_accel_z / 10.0;
    
    Gyro_Radian_Data.X = (float)(MPU6050_Real_Data.Gyro_X  * (3.14159265/180.0));
    Gyro_Radian_Data.Y = (float)(MPU6050_Real_Data.Gyro_Y  * (3.14159265/180.0));
    Gyro_Radian_Data.Z = (float)(MPU6050_Real_Data.Gyro_Z  * (3.14159265/180.0));
}

 


