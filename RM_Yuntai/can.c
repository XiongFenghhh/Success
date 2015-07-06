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
    //ʹ�����ʱ��
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��PORTAʱ��	                   											 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//ʹ��CAN1ʱ��	
	
    //��ʼ��GPIO
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//| GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��PA11,PA12
	
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;//| GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��PA11,PA12
	
	  //���Ÿ���ӳ������
	  GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_CAN1); //GPIOA11����ΪCAN1
	  GPIO_PinAFConfig(GPIOA,GPIO_PinSource12,GPIO_AF_CAN1); //GPIOA12����ΪCAN1
	  
		//CAN��Ԫ����
		CAN_DeInit(CAN1);//added by huangmin
   	CAN_InitStructure.CAN_TTCM=DISABLE;	//��ʱ�䴥��ͨ��ģʽ   
  	//CAN_InitStructure.CAN_ABOM=DISABLE;	//����Զ����߹���	
    CAN_InitStructure.CAN_ABOM=ENABLE;	//����Զ����߹���		
  	//CAN_InitStructure.CAN_AWUM=DISABLE;//˯��ģʽͨ���������(���CAN->MCR��SLEEPλ)
		CAN_InitStructure.CAN_AWUM=ENABLE;//˯��ģʽͨ���������(���CAN->MCR��SLEEPλ)
  	//CAN_InitStructure.CAN_NART=ENABLE;	//��ֹ�����Զ����� 
  	CAN_InitStructure.CAN_NART=DISABLE;	//��ֹ�����Զ����� 
		CAN_InitStructure.CAN_RFLM=DISABLE;	//���Ĳ�����,�µĸ��Ǿɵ�  
  	CAN_InitStructure.CAN_TXFP=DISABLE;	//���ȼ��ɱ��ı�ʶ������ 
  	CAN_InitStructure.CAN_Mode= mode;	 //ģʽ���� 
  	CAN_InitStructure.CAN_SJW=tsjw;	//����ͬ����Ծ���(Tsjw)Ϊtsjw+1��ʱ�䵥λ CAN_SJW_1tq~CAN_SJW_4tq
  	CAN_InitStructure.CAN_BS1=tbs1; //Tbs1��ΧCAN_BS1_1tq ~CAN_BS1_16tq
  	CAN_InitStructure.CAN_BS2=tbs2;//Tbs2��ΧCAN_BS2_1tq ~	CAN_BS2_8tq
  	CAN_InitStructure.CAN_Prescaler=brp;  //��Ƶϵ��(Fdiv)Ϊbrp+1	
  	CAN_Init(CAN1, &CAN_InitStructure);   // ��ʼ��CAN1 
    
		//���ù�����
 	  CAN_FilterInitStructure.CAN_FilterNumber=0;	  //������0
  	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 
  	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; //32λ 
  	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;////32λID
  	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32λMASK
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
   	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//������0������FIFO0
  	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //���������0
  	CAN_FilterInit(&CAN_FilterInitStructure);//�˲�����ʼ��
    
	
    NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);	  
		 
 
	return 0;
}   


//�жϷ�����			    
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
             //�����̨���0x201������ֵ
           current_position_201 = (rx_message.Data[0]<<8) | rx_message.Data[1];		
//				   pWord[0] = rx_message.Data[1];
//					 pWord[1] = rx_message.Data[0];
        }
        if(rx_message.StdId == 0x202)
        { 
					current_position_201 = (rx_message.Data[0]<<8) | rx_message.Data[1];	
					  //�����̨���0x202������ֵ           
        }		
        if(rx_message.StdId == 0x203)
        { 
             //�����̨���0x203������ֵ
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
																					 (int16_t)Position_Control_203(Yaw,wanted_y,0));//����λ�û�
																						//		(int16_t)Position_Control_203(current_position_203,4000,0 );
       realWSpeed= followControl(current_position_203);
//============================================================//		
//			Cmd_ESC((int16_t)Position_Control_201(current_position_201,wanted_p ),
//																																					0,
//			(int16_t)Velocity_Control_203(current_position_203-pre_position_203,Position_Control_203(current_position_203,wanted_y,0)));//λ�ã��ٶȻ�
//			printf("%f %d\r\n",Position_Control_203(current_position_203,wanted_y,0),current_203);
//			Cmd_ESC((int16_t)Position_Control_201(current_position_201,wanted_p ),
//																																					0,
//			(int16_t)Current_Control_203((double)current_203,Position_Control_203(current_position_203,wanted_y,0)));
//			printf("%f \r\n",Current_Control_203((double)current_203,Position_Control_203(current_position_203,wanted_y,0)));//���������
			
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
			CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);//FIFO0��Ϣ�Һ��ж�����.  
  
	} 

//can����һ������(�̶���ʽ:IDΪ0X12,��׼֡,����֡)	
//len:���ݳ���(���Ϊ8)				     
//msg:����ָ��,���Ϊ8���ֽ�.
//����ֵ:0,�ɹ�;
//		 ����,ʧ��;
//u8 CAN1_Send_Msg(u8* msg,u8 len)
//{	
//  u8 mbox;
//  u16 i=0;
//  CanTxMsg TxMessage;
//  TxMessage.StdId=0x200;	 // ��׼��ʶ��Ϊ0
//  TxMessage.IDE=CAN_ID_STD;		  // ʹ����չ��ʶ��
//  TxMessage.RTR=0;		  // ��Ϣ����Ϊ����֡��һ֡8λ
//  TxMessage.DLC=len;							 // ������֡��Ϣ
//  for(i=0;i<len;i++)
//			TxMessage.Data[i]=msg[i];				 // ��һ֡��Ϣ          
//  mbox= CAN_Transmit(CAN1, &TxMessage);   
//  i=0;
//  while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//�ȴ����ͽ���
//  if(i>=0XFFF)return 1;
//  return 0;		

//}
//can�ڽ������ݲ�ѯ
//buf:���ݻ�����;	 
//����ֵ:0,�����ݱ��յ�;
//		 ����,���յ����ݳ���;
u8 CAN1_Receive_Msg(u8 *buf)
{		   		
  u32 i;
	CanRxMsg RxMessage;  
  if( CAN_MessagePending(CAN1,CAN_FIFO0)==0)return 0;		//û�н��յ�����,ֱ���˳� 
    CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);//��ȡ����	
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











