#include "stm32f4xx.h"
#include "delay.h"

void delay_us(u32 n)
{
	u8 j;
	while(n--)
	for(j=0;j<42;j++);		    //�ӳ�����1����ԭΪj<10����72Mʱj<20����108Mʱj<27
}
void delay_ms(u32 n)
{
	while(n--)
	delay_us(42000);
}
void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}

