#include "usart2.h"

/*-----USART2_TX-----PD5-----*/
/*-----USART2_RX-----PD6-----*/

void USART2_Configuration(void)
{
    USART_InitTypeDef usart2;
		GPIO_InitTypeDef  gpio;
    NVIC_InitTypeDef  nvic;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource6,GPIO_AF_USART2); 
	
	gpio.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
	gpio.GPIO_Mode = GPIO_Mode_AF;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
    gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD,&gpio);

	usart2.USART_BaudRate = 115200; 		//@hiyangdong（2015年4月11日 19:16:55）实际波特率是38400，可能与晶振有关吧。
	usart2.USART_WordLength = USART_WordLength_8b;
	usart2.USART_StopBits = USART_StopBits_1;
	usart2.USART_Parity = USART_Parity_No;
	usart2.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;
  usart2.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART2,&usart2);

  USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);  
	
	USART_Cmd(USART2,ENABLE);
 
  
    nvic.NVIC_IRQChannel = USART2_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 1;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
}

void USART2_SendChar(unsigned char b)
{
    while (USART_GetFlagStatus(USART2,USART_FLAG_TC) == RESET);
	USART_SendData(USART2,b);
//	STM_EVAL_LEDOn(LED3);
}



#define DebugCountMax  10
uint8_t DebugBuf[DebugCountMax] ;
uint8_t DebugCnt = 0;
static uint16_t kp;
static uint16_t ki;
static uint16_t kd;
static uint8_t moto_num;
static uint8_t isReceived=0;
void USART2_IRQHandler(void)
{
	
	uint8_t head = 0x7e;
	uint8_t head_Tmp;

	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		DebugBuf[DebugCnt] = USART2->DR;			//acquire data
		DebugCnt++;     
		if(DebugCnt == DebugCountMax) 
		{
			head_Tmp = DebugBuf[0] ;
			if(head_Tmp == head)
			{
				kp = (uint16_t)((DebugBuf[2]<<8) + DebugBuf[1]);
				ki = (uint16_t)((DebugBuf[4]<<8) + DebugBuf[3]);
				kd = (uint16_t)((DebugBuf[6]<<8) + DebugBuf[5]);
				moto_num=DebugBuf[7];
				isReceived=1;
			}
			DebugCnt = 0;
			
		}
  }
}
void resetIsReceive(){isReceived=0;}
uint8_t getIsReceive(){return isReceived;}
void setMotoParameter(){
if(moto_num==201)setPitchPositionParameters(((double)kp)/100,ki,((double)kd)/10);
if(moto_num==203)setYawPositionParameters(((double)kp)/100,ki,((double)kd)/10);
}
int fputc(int ch, FILE *f)
{
    while (USART_GetFlagStatus(USART2,USART_FLAG_TC) == RESET);
    USART_SendData(USART2, (uint8_t)ch);
    return ch;
}


