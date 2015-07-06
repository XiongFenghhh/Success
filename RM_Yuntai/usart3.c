//#include "RMVariables.h"
#include "usart3.h"

/*-----USART3_TX-----PB10-----*/
/*-----USART3_RX-----PB11-----*/

void USART3_Configuration(void)
{
    USART_InitTypeDef usart3;
	GPIO_InitTypeDef  gpio;
    NVIC_InitTypeDef  nvic;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); 
	
	gpio.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	gpio.GPIO_Mode = GPIO_Mode_AF;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
    gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB,&gpio);

	usart3.USART_BaudRate = 115200; 		//@hiyangdong（2015年4月11日 19:16:55）实际波特率是38400，可能与晶振有关吧。
	usart3.USART_WordLength = USART_WordLength_8b;
	usart3.USART_StopBits = USART_StopBits_1;
	usart3.USART_Parity = USART_Parity_No;
	usart3.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;
    usart3.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART3,&usart3);

 USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);  
	
	USART_Cmd(USART3,ENABLE);
    
    nvic.NVIC_IRQChannel = USART3_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 1;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
}

void USART3_SendChar(unsigned char b)
{
  USART_SendData(USART3,b);  
	while (USART_GetFlagStatus(USART3,USART_FLAG_TC) == RESET){};
	
//	STM_EVAL_LEDOn(LED3);
}



#define RxCountMax  10
uint8_t RxBuf[RxCountMax] ;

uint8_t RxCnt = 0;
static int16_t Yaw_adjust   = 0;
static int16_t Pitch_adjust = 0;
static uint8_t adjust_flag = 1;
#define DebugCountMax  10
static uint8_t DebugBuf[DebugCountMax] ;
static uint8_t DebugCnt = 0;
static uint16_t kp;
static uint16_t ki;
static uint16_t kd;
static uint8_t moto_num;
static uint8_t isReceived;
static uint8_t head=0x7e;
static uint8_t special1=0;
static uint8_t special2=0;
static uint8_t change=0;

uint8_t HtoEs_OutPut_Buffer[15];
unsigned int CHK_SUM;  //校验和
float Pitch_Val;
float Roll_Val;
float Yaw_Val;
u8 data_to_send[50];	// 

void USART3_IRQHandler(void)
{
	
//	uint16_t head = 0xffff;
//	uint16_t head_Tmp;

//	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
//	{
//		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
//		RxBuf[RxCnt] = USART3->DR;;				//acquire data
//		RxCnt++;     
//		if(RxCnt == RxCountMax) 
//		{
//			head_Tmp = (uint16_t)((RxBuf[0]<<8) + RxBuf[1]);
//			if(head_Tmp == head)
//			{
//				Yaw_adjust   = (int16_t)((RxBuf[2]<<8) + RxBuf[3] - 1)*((RxBuf[4]<<8) + RxBuf[5]);
//				Pitch_adjust = (int16_t)((RxBuf[6]<<8) + RxBuf[7] - 1)*((RxBuf[8]<<8) + RxBuf[9]);
//				adjust_flag  = 1;
//				
//			}
//			RxCnt = 0;
//			
//		}
//  }


	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		uint8_t temp;
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
		temp =(uint8_t)((USART3->DR));			//acquire data
		
		if(temp==0x7e&&DebugCnt==0)
		{
			DebugBuf[0]=temp; 
			DebugCnt++;
		}else if(DebugBuf[0]==0x7e)
		{
			if(temp==0x7e)
			{
					kp=DebugBuf[1]|DebugBuf[2]<<8;
					ki=DebugBuf[3]|DebugBuf[4]<<8;
					kd=DebugBuf[5]|DebugBuf[6]<<8;
					moto_num=DebugBuf[7];
					isReceived=1;
					DebugCnt=0;
					DebugBuf[0]=0x00;
			}else if(temp==0x7d)
			{
					special1=1;
			}else if(special1)
			{
					if(temp==0x5e)
					{
						DebugBuf[DebugCnt++]=0x7e;
						special1=0;
					}
					else if(temp==0x5d)
					{
						DebugBuf[DebugCnt++]=0x7d;
						special1=0;
					}else
					{
						DebugBuf[DebugCnt++]=0x7d;
						DebugBuf[DebugCnt++]=temp;
					}
			}else
			{
					DebugBuf[DebugCnt++]=temp;
			}
			
		}
		
  
}
}
void resetIsReceive(){isReceived=0;}
uint8_t getIsReceive(){return isReceived;}
void setMotoParameter(){
//	int kpi;
////setYawPositionParameters(kp/100.0,0,kd/10.0);
//if(kd==1)kpi=-(int)kp;
//	else kpi=kp;
//if(ki==2)
//	{
//		setWanted_pWanted_y(kp,kd);
//	}
//if(kd==3)
//	{
//		 me.Kp[0]=(double)(kp)/100;
//		me.Ki[0]=(double)(ki)/1000;
//	}
// if(kd==4)
// {
//	 me.Kp[1]=(double)(kp)/100;
//	 me.Ki[1]=(double)(ki)/1000;
// }
// if(kd==5)
// {
//	 me.Kp[2]=(double)(kp)/100;
//	 me.Ki[2]=(double)(ki)/1000;
// }
// if(kd==6)
// {
	 Kp=(double)(kp)/100;
	 Ki=(double)(ki)/1000;
	 Kd=(double)(kd)/100;
// }
// if(ki==7)
// {
//	 setDeadTime(kp);
// }
}
int16_t getYunTaiAdjustYaw(){return Yaw_adjust ;}
int16_t getYunTaiAdjustPitch(){return Pitch_adjust ;}
/*
*@author hiyangdong
*by redefining std::fputc and using microlib,we can solve the problem with printf in c++!!
*/

int fputc(int ch, FILE *f)
{
    while (USART_GetFlagStatus(USART3,USART_FLAG_TC) == RESET);
    USART_SendData(USART3, (uint8_t)ch);
    return ch;
}
static u16 RS232_VisualScope_CRC16( u8 *Array, u16 Len )
{
	u16 USART_IX, USART_IY, USART_CRC;

	USART_CRC = 0xffff;
	for(USART_IX=0; USART_IX<Len; USART_IX++) {
		USART_CRC = USART_CRC^(u16)(Array[USART_IX]);
		for(USART_IY=0; USART_IY<=7; USART_IY++) {
			if((USART_CRC&1)!=0)
				USART_CRC = (USART_CRC>>1)^0xA001;
			else
				USART_CRC = USART_CRC>>1;
		}
	}
	return(USART_CRC);
}



void RS232_VisualScope( USART_TypeDef* USARTx, u8 *pWord, u16 Len )
{
	u8 i = 0;
	u16 Temp = 0;

	Temp = RS232_VisualScope_CRC16(pWord, Len);
	pWord[8] = Temp&0x00ff;
	pWord[9] = (Temp&0xff00)>>8;

	for(i=0; i<10; i++) {
		USART_SendData(USARTx, (uint8_t)*pWord);
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
		pWord++;
	}
}

void ANO_DT_Send_Data(u8 *dataToSend , u8 length)
{ 
	  u8 i ;
	  for(i=0; i<length; i++) {
		USART_SendData(USART3, (uint8_t)*dataToSend);
		while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
		dataToSend++;
}
}

void ANO_DT_Send_Status(float angle_rol, float angle_pit, float angle_yaw, s32 alt, u8 fly_model, u8 armed)
{
	u8 _cnt=0;u8 sum = 0;u8 i = 0;
	vs16 _temp;
	vs32 _temp2 = alt;
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x01;
	data_to_send[_cnt++]=0;
	
	_temp = (int)(angle_rol*100);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)(angle_pit*100);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)(angle_yaw*100);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	data_to_send[_cnt++]=BYTE3(_temp2);
	data_to_send[_cnt++]=BYTE2(_temp2);
	data_to_send[_cnt++]=BYTE1(_temp2);
	data_to_send[_cnt++]=BYTE0(_temp2);
	
	data_to_send[_cnt++] = fly_model;
	
	data_to_send[_cnt++] = armed;
	
	data_to_send[3] = _cnt-4;
	
	for(i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++]=sum;
	
	ANO_DT_Send_Data(data_to_send, _cnt);
} 

void Float2Byte(float *target,unsigned char *buf,unsigned char beg)
{
    unsigned char *point;
    point = (unsigned char*)target;	  //得到float的地址
    buf[beg]   = point[0];
    buf[beg+1] = point[1];
    buf[beg+2] = point[2];
    buf[beg+3] = point[3];
}

unsigned char HtoEs_Attitude_Data_Generate(void)
{
	  unsigned char i;
	
	  HtoEs_OutPut_Buffer[0] = 0x0F; //帧长度 15字节
		HtoEs_OutPut_Buffer[1] = 0x03; //功能码
	
	  Float2Byte(&Pitch_Val ,HtoEs_OutPut_Buffer,2);  //俯仰
	  Float2Byte(&Roll_Val  ,HtoEs_OutPut_Buffer,6);  //横滚
	  Float2Byte(&Yaw_Val   ,HtoEs_OutPut_Buffer,10); //航向
	  
	//============================================================================	
		
		CHK_SUM =0; 
	
	  for(i = 0 ; i < 14; i++)  //计算和
			CHK_SUM += HtoEs_OutPut_Buffer[i];
		
		HtoEs_OutPut_Buffer[14] = CHK_SUM % 255; //计算校验值
		
	  return 15; 
}

void yuandian_doc_display() //圆点博士上位机协议
{
	unsigned char dis[32];
	unsigned char euler_data_sign =0xB0;
	 int euler_data[3];
	u8 i = 0;
	unsigned char data_buf[8];
	unsigned char data_send[18];
	unsigned char SUM;
	int p,r,y;
	 		p=(int)(Pitch*100); r=(int)(Roll*100); y=(int)(Yaw*100);
		if(p<0){
		  euler_data_sign |=4;
			p+=18000;
		}
		if(r<0){
		  euler_data_sign |=2;
			r+=18000;
		}
		if(y<0){
		  euler_data_sign |=1;
			y+=18000;
		}
		euler_data[0] = p;euler_data[1]=r;euler_data[2]=y;
		data_buf[0] = euler_data_sign;
		for(i=0;i<3;i++){
		data_buf[2*i+1]=euler_data[i]>>8 & 0x00ff;
		data_buf[2*i+2]=euler_data[i]& 0x00ff;	
		}
		SUM =euler_data_sign;
		for(i=0;i<6;i++)
		{
			SUM+=data_buf[i];
		}
		SUM = 0x100-SUM;
		data_buf[7] = SUM;
		data_send[0]=0x3A;
		
		for(i=0;i<8;i++)
		{
			data_send[2*i+1]=data_buf[i]>>4 &0x0f;
			data_send[2*i+2]=data_buf[i]&0x0f;
			if(data_send[2*i+1]<10) data_send[2*i+1]+=0x30;
			else data_send[2*i+1]+=55;
			if(data_send[2*i+2]<10) data_send[2*i+2]+=0x30;
			else  data_send[2*i+2]+=55;
		} 
		//data_send[17] = SUM;
		data_send[17] = 0x2F;
//if(itTimes%2==0){
	 for( i = 0 ; i < 18; i++)  //????,??????   
	 	  {
		    while((USART3->SR&0X40)==0);  
  	    USART3->DR = data_send[i]; //????????????      
		  }
}