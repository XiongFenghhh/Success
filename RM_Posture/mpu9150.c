#include "AHRS_Attitude.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "STM32_I2C.h"
#include <stdio.h>
#include "mpu9150.h"

#define DEFAULT_MPU_HZ  1000

void init_mpu9150(void)
{
  int result;
  unsigned char data_write[1];
	char mpu_init_time = 0;
	result = mpu_init();
	//@moodified by huangmin
  while(result && mpu_init_time<16)
	{
		mpu_init_time++;
		result = mpu_init();
		printf("try mpu_init() one more time ......\r");
	}
  
  if(!result)
  {
	  //mpu_init();
	  printf("\r\n mpu initialization complete ......\n\r ");

	  //�������ٶȼơ������ǡ������ƣ�ʹ�õ������̣�Ҫ����INV_XYZ_COMPASS
	  if(!mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL | INV_XYZ_COMPASS))
	  {
	  	 printf("\r\n mpu_set_sensor complete ......\n\r");
	  }
	  else
	  {
	  	 printf("\r\n mpu_set_sensor error ......\n\r");
		 //GPIO_SetBits(GPIOE, GPIO_Pin_7);
		 while(1);
	  }

	  //����MPU9150��ʱ��ԴΪGX��PLL
	  data_write[0]=0x01;				//GX_PLL:0x01
	  if(!i2cwrite(MPU9150_Addr, P_M_1, 1, data_write))
	  {
	  	 printf("\r\n set_mpu9150_ClockSource complete ......\n\r");
	  }
	  else
	  {
	  	 printf("\r\n set_mpu9150_ClockSource error ......\n\r");
		 //GPIO_SetBits(GPIOE, GPIO_Pin_7);
		 while(1);
	  }

	  //���������ǲ�������+-500��/s
		//modified by huangmin on 20150628( important!!)
	  if(!mpu_set_gyro_fsr(500))
	  {
	  	 printf("\r\n mpu_set_gyro_fsr complete ......\n\r");
	  }
	  else
	  {
	  	 printf("\r\n mpu_set_gyro_fsr error ......\n\r");
		 //GPIO_SetBits(GPIOE, GPIO_Pin_7);
		 while(1);
	  }

	  //���ü��ٶȼƲ�����Χ��+-4G
	  if(!mpu_set_accel_fsr(4))
	  {
	  	 printf("\r\n mpu_set_accel_fsr complete ......\n\r");
	  }
	  else
	  {
	  	 printf("\r\n mpu_set_accel_fsr error ......\n\r");
		 //GPIO_SetBits(GPIOE, GPIO_Pin_7);
		 while(1);
	  }

	  //���ü��ٶȼƵĵ�ͨ�˲��������𶯣���������X2212 kv980�����ѹ11.1v��ת��Ϊ120r/s�����DLPFѡ98hz������ɿ��Ǽ�Ȩƽ���˲�
	  if(!mpu_set_lpf(Accel_FILTER_98HZ))
	  {
	  	 printf("\r\n mpu_set_lpf complete ......\n\r");
	  }
	  else
	  {
	  	 printf("\r\n mpu_set_lpf error ......\n\r");
		 //GPIO_SetBits(GPIOE, GPIO_Pin_7);
		 while(1);
	  }

	  //���ò�����1kHz
	  if(!mpu_set_sample_rate(DEFAULT_MPU_HZ))
	  {
	  	 printf("\r\n mpu_set_sample_rate complete ......\n\r");
	  }
	  else
	  {
	  	 printf("\r\n mpu_set_sample_rate error ......\n\r");
		 //GPIO_SetBits(GPIOE, GPIO_Pin_7);
		 while(1);
	  }
	  
	  //ͨ��5000�ξ��ü�Ȩƽ�������������ƫOFFSET
	  if(!get_gyro_bias())
	  {
	  	 printf("\r\n get_gyro_bias complete ......\n\r");
	  }
	  else
	  {
	  	 printf("\r\n get_gyro_bias error ......\n\r");
		 //GPIO_SetBits(GPIOE, GPIO_Pin_7);
		 while(1);
	  }

	  //�ȶ�����mag���ݣ���Ϊǰ����mag�����д���оƬbug
	  if(!Init_MPU9150_Mag())
	  {
	  	 printf("\r\n Init_MPU9150_Mag complete ......\n\r");
	  }
	  else
	  {
	  	 printf("\r\n Init_MPU9150_Mag error ......\n\r");
		 //GPIO_SetBits(GPIOE, GPIO_Pin_7);
		 while(1);
	  }
  }
  else
  {
      printf("\r\n mpu initialization error......\n\r ");
      //GPIO_SetBits(GPIOE, GPIO_Pin_7);
	  while(1);
  } 
  printf("\r\n Start Calculating .....\n\r");
}
