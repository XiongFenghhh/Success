#include "STM32_I2C.h"

//#define SCL_H         GPIO_SetBits(GPIOB,GPIO_Pin_8)
//#define SCL_L         GPIO_ResetBits(GPIOB,GPIO_Pin_8)
#define SCL_H         GPIO_SetBits(GPIOB,GPIO_Pin_6)
#define SCL_L         GPIO_ResetBits(GPIOB,GPIO_Pin_6)

//#define SDA_H         GPIO_SetBits(GPIOB,GPIO_Pin_9)
//#define SDA_L         GPIO_ResetBits(GPIOB,GPIO_Pin_9)
#define SDA_H         GPIO_SetBits(GPIOB,GPIO_Pin_7)
#define SDA_L         GPIO_ResetBits(GPIOB,GPIO_Pin_7)

////#define SCL_read      GPIOB->IDR  & GPIO_Pin_10 /* GPIO_ReadInputDataBit(GPIOB , GPIO_Pin_10) */
#define SDA_read      GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)
//#define SDA_read      GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)
void I2C_SDA_Out(void)
{
    GPIO_InitTypeDef   gpio;
    
	gpio.GPIO_Pin = GPIO_Pin_7;
//	gpio.GPIO_Pin = GPIO_Pin_9;
    gpio.GPIO_Mode = GPIO_Mode_OUT;
	gpio.GPIO_OType = GPIO_OType_OD;
	gpio.GPIO_Speed = GPIO_Speed_100MHz; 
    GPIO_Init(GPIOB, &gpio);
}

void I2C_SDA_In(void)
{
    GPIO_InitTypeDef   gpio;
    
	gpio.GPIO_Pin = GPIO_Pin_7;
//	gpio.GPIO_Pin = GPIO_Pin_9;
    
    gpio.GPIO_Mode = GPIO_Mode_IN;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpio);
}

static void I2C_delay(void)
{
    volatile int i = 7;	  //原为i=7，使用GD32芯片，延迟需增加1倍，以满足时序，72M时i=14，108M时i=20
    while (i)
        i--;
}

static bool I2C_Start(void)
{
	  I2C_SDA_Out();
    SDA_H;
    SCL_H;
    I2C_delay();
    if (!SDA_read)
        return false;
    SDA_L;
    I2C_delay();
    if (SDA_read)
        return false;
    SDA_L;
    I2C_delay();
    return true;
}

static void I2C_Stop(void)
{
	  I2C_SDA_Out();
    SCL_L;
    I2C_delay();
    SDA_L;
    I2C_delay();
    SCL_H;
    I2C_delay();
    SDA_H;
    I2C_delay();
}

static void I2C_Ack(void)
{
	  I2C_SDA_Out();
    SCL_L;
    I2C_delay();
    SDA_L;
    I2C_delay();
    SCL_H;
    I2C_delay();
    SCL_L;
    I2C_delay();
}

static void I2C_NoAck(void)
{
	  I2C_SDA_Out();
    SCL_L;
    I2C_delay();
    SDA_H;
    I2C_delay();
    SCL_H;
    I2C_delay();
    SCL_L;
    I2C_delay();
}

static bool I2C_WaitAck(void)
{
    SCL_L;
    I2C_delay();
    SDA_H;
	  I2C_SDA_In();
    I2C_delay();
    SCL_H;
    I2C_delay();
    if (SDA_read) {
        SCL_L;
        return false;
    }
    SCL_L;
    return true;
}

static void I2C_SendByte(uint8_t byte)
{
	  uint8_t i = 8;
    I2C_SDA_Out();
    while (i--) {
        SCL_L;
        I2C_delay();
        if (byte & 0x80)
            SDA_H;
        else
            SDA_L;
        byte <<= 1;
        I2C_delay();
        SCL_H;
        I2C_delay();
    }
    SCL_L;
}

static uint8_t I2C_ReceiveByte(void)
{
    uint8_t i = 8;
    uint8_t byte = 0;
    I2C_SDA_In();
    SDA_H;
    while (i--) {
        byte <<= 1;
        SCL_L;
        I2C_delay();
        SCL_H;
        I2C_delay();
        if (SDA_read) {
            byte |= 0x01;
        }
    }
    SCL_L;
    return byte;
}
/*----I2C1----SCL----PB6---*/
/*----I2C1----SDA----PB7---*/
void i2cInit(void)
{
    GPIO_InitTypeDef   gpio;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	  gpio.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_6;
	//gpio.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_8;
		gpio.GPIO_Mode = GPIO_Mode_OUT;
		gpio.GPIO_OType = GPIO_OType_PP;
		gpio.GPIO_Speed = GPIO_Speed_100MHz; 
    GPIO_Init(GPIOB, &gpio);
	  
}

bool i2cWriteBuffer(uint8_t addr, uint8_t reg, uint8_t len, uint8_t * data)
{
    int i;
    if (!I2C_Start())
        return false;
    I2C_SendByte(addr << 1 | I2C_Direction_9150Transmitter);
    if (!I2C_WaitAck()) {
        I2C_Stop();
        return false;
    }
    I2C_SendByte(reg);
    I2C_WaitAck();
    for (i = 0; i < len; i++) {
        I2C_SendByte(data[i]);
        if (!I2C_WaitAck()) {
            I2C_Stop();
            return false;
        }
    }
    I2C_Stop();
    return true;
}
/////////////////////////////////////////////////////////////////////////////////
int8_t i2cwrite(uint8_t addr, uint8_t reg, uint8_t len, uint8_t * data)
{
	if(i2cWriteBuffer(addr,reg,len,data))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	//return FALSE;
}
int8_t i2cread(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
	if(i2cRead(addr,reg,len,buf))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	//return FALSE;
}
//////////////////////////////////////////////////////////////////////////////////
bool i2cWrite(uint8_t addr, uint8_t reg, uint8_t data)
{
    if (!I2C_Start())
        return false;
    I2C_SendByte(addr << 1 | I2C_Direction_Transmitter);
    if (!I2C_WaitAck()) {
        I2C_Stop();
        return false;
    }
    I2C_SendByte(reg);
    I2C_WaitAck();
    I2C_SendByte(data);
    I2C_WaitAck();
    I2C_Stop();
    return true;
}

bool i2cRead(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
    if (!I2C_Start())
        return false;
    I2C_SendByte(addr << 1 | I2C_Direction_Transmitter);
    if (!I2C_WaitAck()) {
        I2C_Stop();
        return false;
    }
    I2C_SendByte(reg);
    I2C_WaitAck();
    I2C_Start();
    I2C_SendByte(addr << 1 | I2C_Direction_Receiver);
    I2C_WaitAck();
    while (len) {
        *buf = I2C_ReceiveByte();
        if (len == 1)
            I2C_NoAck();
        else
            I2C_Ack();
        buf++;
        len--;
    }
    I2C_Stop();
    return true;
}

uint16_t i2cGetErrorCounter(void)
{
    // TODO maybe fix this, but since this is test code, doesn't matter.
    return 0;
}
