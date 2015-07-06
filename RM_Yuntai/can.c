#include "can.h"
#ifdef Shooter1
static double wanted_p=4000;//ps:5500 ss:4000
static double prewanted_p=4000;//ps:5500 ss:4000
static double wanted_y= 0;//modified by huangmin 20150628
//static double wanted_y=4200;//ps:5000 ss:3000
static double prewanted_y=4200;//ps:5000 ss:3000
#endif
#ifdef Shooter2
static double wanted_p=4000;//ps:5500 ss:4000
static double prewanted_p=4000;//ps:5500 ss:4000
static double wanted_y= 0;//modified by huangmin 20150628
//static double wanted_y=4000;//ps:5000 ss:3000
static double prewanted_y=4000;
#endif
#ifdef Gun
static double wanted_p=5500;//ps:5500 ss:4000
static double prewanted_p=5500;//ps:5500 ss:4000
static double wanted_y=5000;//ps:5000 ss:3000
static double prewanted_y=5000;
#endif
static uint16_t canItTimes=0;
static double target_p=0;
static double target_y=0;
static uint8_t isAutoTargetMode;
static int8_t isYawTargeted=0;
static int8_t isPitchTargeted=0;
static CanRxMsg rx_message;
int16_t current_203;

int16_t current_position_201;
static int pre_position_203;
static int current_position_203;//@modified by huangmin on 2015.04.17 
int can_irq_count = 0;//added by huangmin on 2015.04.25 
void printCurrentPosition(void){printf("%d\r\n",current_position_203);}
/*if auto mode find target, record the position the moment target found*/
void setWanted_pWanted_y(uint16_t wp,uint16_t wy){
	wanted_p=wp;
	wanted_y=wy;
}

u8 CAN1_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode)
{
	
  		GPIO_InitTypeDef      GPIO_InitStructure; 
	  CAN_InitTypeDef       CAN_InitStructure;
  	CAN_FilterInitTypeDef CAN_FilterInitStructure; 
   	NVIC_InitTypeDef      NVIC_InitStructure; 
    //使能相关时钟
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能PORTA时钟	                   											 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//使能CAN1时钟	
	
    //初始化GPIO
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//| GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化PA11,PA12
	
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;//| GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化PA11,PA12
	
	  //引脚复用映射配置
	  GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_CAN1); //GPIOA11复用为CAN1
	  GPIO_PinAFConfig(GPIOA,GPIO_PinSource12,GPIO_AF_CAN1); //GPIOA12复用为CAN1
	  
		//CAN单元设置
		CAN_DeInit(CAN1);//added by huangmin
   	CAN_InitStructure.CAN_TTCM=DISABLE;	//非时间触发通信模式   
  	//CAN_InitStructure.CAN_ABOM=DISABLE;	//软件自动离线管理	
    CAN_InitStructure.CAN_ABOM=ENABLE;	//软件自动离线管理		
  	//CAN_InitStructure.CAN_AWUM=DISABLE;//睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
		CAN_InitStructure.CAN_AWUM=ENABLE;//睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
  	//CAN_InitStructure.CAN_NART=ENABLE;	//禁止报文自动传送 
  	CAN_InitStructure.CAN_NART=DISABLE;	//禁止报文自动传送 
		CAN_InitStructure.CAN_RFLM=DISABLE;	//报文不锁定,新的覆盖旧的  
  	CAN_InitStructure.CAN_TXFP=DISABLE;	//优先级由报文标识符决定 
  	CAN_InitStructure.CAN_Mode= mode;	 //模式设置 
  	CAN_InitStructure.CAN_SJW=tsjw;	//重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位 CAN_SJW_1tq~CAN_SJW_4tq
  	CAN_InitStructure.CAN_BS1=tbs1; //Tbs1范围CAN_BS1_1tq ~CAN_BS1_16tq
  	CAN_InitStructure.CAN_BS2=tbs2;//Tbs2范围CAN_BS2_1tq ~	CAN_BS2_8tq
  	CAN_InitStructure.CAN_Prescaler=brp;  //分频系数(Fdiv)为brp+1	
  	CAN_Init(CAN1, &CAN_InitStructure);   // 初始化CAN1 
    
		//配置过滤器
 	  CAN_FilterInitStructure.CAN_FilterNumber=0;	  //过滤器0
  	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 
  	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; //32位 
  	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;////32位ID
  	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32位MASK
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
   	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//过滤器0关联到FIFO0
  	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //激活过滤器0
  	CAN_FilterInit(&CAN_FilterInitStructure);//滤波器初始化
    
	
    NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);	  
		 
 
	return 0;
}   


//中断服务函数			    
void CAN1_RX0_IRQHandler(void)
{
	 canItTimes++;
	 canItTimes%=5;
	  //@modified by huangmin on 2015.04.25 
    CAN_ClearITPendingBit(CAN1,CAN_IT_FMP0); //@modified by huangmin on 2015.04.17
	  CAN_ITConfig(CAN1, CAN_IT_FMP0, DISABLE); //disable can_fmp0 interrupt 
	if(canItTimes==0){  
	 CAN_Receive(CAN1, CAN_FIFO0, &rx_message);
                   
        if(rx_message.StdId == 0x201)
        {             
             //获得云台电机0x201的码盘值
           current_position_201 = (rx_message.Data[0]<<8) | rx_message.Data[1];		
//				   pWord[0] = rx_message.Data[1];
//					 pWord[1] = rx_message.Data[0];
        }
        if(rx_message.StdId == 0x202)
        { 
					current_position_201 = (rx_message.Data[0]<<8) | rx_message.Data[1];	
					  //获得云台电机0x202的码盘值           
        }		
        if(rx_message.StdId == 0x203)
        { 
             //获得云台电机0x203的码盘值
					//@modified by huangmin on 2015.04.25
				  current_203 = (rx_message.Data[2]<<8) | rx_message.Data[3];
//					pWord[2] = rx_message.Data[3];
//					pWord[3] = rx_message.Data[2];
					//@modified by huangmin on 2015.04.17
					current_position_203 = (rx_message.Data[0]<<8) | rx_message.Data[1];
//					pWord[4] = rx_message.Data[1];
//				  pWord[5] = rx_message.Data[0];
          vscope_en = 1;
        }
       
	 if(yunTaiProtectionMode==0){				
		//do the mannual control if not in auto mode		
      if(isAutoTargetMode==0)
		 { 
		    //if(abs(RC_Ctl.mouse.x)<1)
			   // wanted_y = wanted_y ;
			 // else
			 if(!(current_position_203>5200&&RC_Ctl.velocity.w<0)&&(!(current_position_203<3200&&RC_Ctl.velocity.w>0)))
		   wanted_y  -= RC_Ctl.velocity.w; 
			   //wanted_y+=RC_Ctl.velocity.w;
		    if(wanted_p>4500)			 wanted_p=4500;//ss:4500 ps:6000
				else if(wanted_p<3300) wanted_p=3300;//ss:3500 ps:5000 
			
			Cmd_ESC((int16_t)Position_Control_201(current_position_201,wanted_p ),
																																					0,
																					 (int16_t)Position_Control_203(Yaw,wanted_y,0));//仅有位置环
																						//		(int16_t)Position_Control_203(current_position_203,4000,0 );
       realWSpeed= followControl(current_position_203);
//============================================================//		
//			Cmd_ESC((int16_t)Position_Control_201(current_position_201,wanted_p ),
//																																					0,
//			(int16_t)Velocity_Control_203(current_position_203-pre_position_203,Position_Control_203(current_position_203,wanted_y,0)));//位置，速度环
//			printf("%f %d\r\n",Position_Control_203(current_position_203,wanted_y,0),current_203);
//			Cmd_ESC((int16_t)Position_Control_201(current_position_201,wanted_p ),
//																																					0,
//			(int16_t)Current_Control_203((double)current_203,Position_Control_203(current_position_203,wanted_y,0)));
//			printf("%f \r\n",Current_Control_203((double)current_203,Position_Control_203(current_position_203,wanted_y,0)));//加入电流环
			
			target_p=current_position_201;
			target_y=current_position_203;
		}
	//	printf(" %d\r\n",isAutoTarget());
		if(isAutoTargetMode==1)
			{
			
				//if the target isn't close to the	center, do auto target
			  //isTargeted is changed by setIsYawTargeted(int8_t),setIsPitchTargeted(int8_t)
						target_p+=0.2*isPitchTargeted;
						target_y+=0.2*isYawTargeted;
			
				if(target_p>4500)  target_p=4500;
				else if(target_p<3500)  	target_p=3500;
				if(target_y>4000)  target_y=4000;
				else if(target_y<2000)    target_y=2000;
			  Cmd_ESC((int16_t)Position_Control_201(current_position_201,target_p),\
																																				   0,\
																							(int16_t)Position_Control_203(current_position_203,target_y,0));
						wanted_p=current_position_201;
						wanted_y=current_position_203;
				 }
					
	     prewanted_p=wanted_p;
       prewanted_y=wanted_y;
       pre_position_203=current_position_203;
				}
else
	   {
				Cmd_ESC((int16_t)Position_Control_201(current_position_201,current_position_201 ),
																																												0,
								(int16_t)Position_Control_203(current_position_203,current_position_203,0));
			}
 }
	
			CAN_FIFORelease(CAN1,CAN_FIFO0);//@modified by huangmin on 2015.04.17
			CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);//FIFO0消息挂号中断允许.  
  
	} 

//can发送一组数据(固定格式:ID为0X12,标准帧,数据帧)	
//len:数据长度(最大为8)				     
//msg:数据指针,最大为8个字节.
//返回值:0,成功;
//		 其他,失败;
//u8 CAN1_Send_Msg(u8* msg,u8 len)
//{	
//  u8 mbox;
//  u16 i=0;
//  CanTxMsg TxMessage;
//  TxMessage.StdId=0x200;	 // 标准标识符为0
//  TxMessage.IDE=CAN_ID_STD;		  // 使用扩展标识符
//  TxMessage.RTR=0;		  // 消息类型为数据帧，一帧8位
//  TxMessage.DLC=len;							 // 发送两帧信息
//  for(i=0;i<len;i++)
//			TxMessage.Data[i]=msg[i];				 // 第一帧信息          
//  mbox= CAN_Transmit(CAN1, &TxMessage);   
//  i=0;
//  while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//等待发送结束
//  if(i>=0XFFF)return 1;
//  return 0;		

//}
//can口接收数据查询
//buf:数据缓存区;	 
//返回值:0,无数据被收到;
//		 其他,接收的数据长度;
u8 CAN1_Receive_Msg(u8 *buf)
{		   		
  u32 i;
	CanRxMsg RxMessage;  
  if( CAN_MessagePending(CAN1,CAN_FIFO0)==0)return 0;		//没有接收到数据,直接退出 
    CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);//读取数据	
    for(i=0;i<RxMessage.DLC;i++)
    buf[i]=RxMessage.Data[i];
  
	return RxMessage.DLC;	
}

//called by: RC_Convert()
void setIsAutoTargetMode(uint8_t i){
isAutoTargetMode=i;
}
void setYunTaiPosition(double wp,double wy){

wanted_p-=wp;
wanted_y-=wy;
}



//both called by main()
void setIsYawTargeted(int8_t i){
isYawTargeted=i;
}

void setIsPitchTargeted(int8_t i){
isPitchTargeted=i;
}











