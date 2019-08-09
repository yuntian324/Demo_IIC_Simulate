/**************************************************START OF FILE*****************************************************/

/*------------------------------------------------------------------------------------------------------------------
包含头文件
*/
#include "stm32f2xx_hal.h"
#include "xBSP_I2C_Simu.h"
#include "xBSP_IO_Map.h"

/*------------------------------------------------------------------------------------------------------------------
宏定义
*/

//IO方向设置
#define SDA_IN()  {xBSP_IIC_SDA_Input_Init();}//PB11,上下拉输入
#define SDA_OUT() {xBSP_IIC_SDA_Output_Init();}//PB11,开漏输出

//IO操作
#define IIC_SCL   PBout(10) //SCL
#define IIC_SDA   PBout(11) //SDA
#define READ_SDA  PBin(11)  //输入SDA


/*------------------------------------------------------------------------------------------------------------------
函数声明
*/
void xBSP_IIC_SDA_Output_Init(void);
void xBSP_IIC_SDA_Input_Init(void);
void xBSP_I2C_Simu_Start(void);				//发送IIC开始信号
void xBSP_I2C_Simu_Stop(void);	  			//发送IIC停止信号
void xBSP_I2C_Simu_Ack(void);					//IIC发送ACK信号
void xBSP_I2C_Simu_NAck(void);				//IIC不发送ACK信号
uint8_t xBSP_I2C_Simu_Wait_Ack(void); 				//IIC等待ACK信号


/*
***************************************************************************************************************
@ Name   : xBSP_I2C_Simu_Init

@ Brief  : IIC初始化

@ Param  : NONE

@ Return : NONE

@ Author : WZB

@  Date  : 
***************************************************************************************************************
*/
void xBSP_I2C_Simu_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOB_CLK_ENABLE();
    
    GPIO_Initure.Pin=GPIO_PIN_10|GPIO_PIN_11;
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;
    GPIO_Initure.Pull=GPIO_PULLUP;
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
    
    IIC_SDA=1;
    IIC_SCL=1;  
}

/*
***********************************************************************************************************************
@函数名称    ：	void xBSP_I2C_Simu_delay_us(void)

@功    能    ：	微秒延时

@参    数    ：	uint32_t nus

@返 回 值    ：	NONE

@作    者    ：	WZB

系 统时钟    ： 72MHz

NOP 个 数    ：  67

示波器实测数据： 
                1nus:       1.185us
                2nus:       2.18us
                10nus:      10.05us
                100nus:     99us
                1000nus:    1005us
************************************************************************************************************************
*/
void xBSP_I2C_Simu_delay_us(uint32_t nus)
{
    while(nus--)
    {
        __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
        __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
        __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
        __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
        __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
        __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
        __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
    }
}

/*
***************************************************************************************************************
@ Name   : xBSP_IIC_SDA_Output_Init

@ Brief  : IIC SDA引脚配置为输出模式

@ Param  : NONE

@ Return : NONE

@ Author : WZB

@  Date  : 2019 - 04 - 26
***************************************************************************************************************
*/
void xBSP_IIC_SDA_Output_Init(void)
{
	static GPIO_InitTypeDef GPIO_InitStructOutput = 
	{
		.Mode = GPIO_MODE_OUTPUT_OD,
		.Pull = GPIO_NOPULL,
		.Speed = GPIO_SPEED_FAST,
		.Pin = GPIO_PIN_11,
	};
	//**开漏输出*****
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructOutput);
}

/*
***************************************************************************************************************
@ Name   : xBSP_IIC_SDA_Output_Init

@ Brief  : IIC SDA引脚配置为输入模式

@ Param  : NONE

@ Return : NONE

@ Author : WZB

@  Date  : 2019 - 04 - 26
***************************************************************************************************************
*/
void xBSP_IIC_SDA_Input_Init(void)
{
	static GPIO_InitTypeDef GPIO_InitStructInput = 
	{
		.Mode = GPIO_MODE_INPUT,
		.Pull = GPIO_NOPULL,
		.Pin = GPIO_PIN_11,
	};
	//**开漏输出*****
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructInput);
}

/*
***************************************************************************************************************
@ Name   : xBSP_I2C_Simu_Start

@ Brief  : IIC 产生IIC起始信号

@ Param  : NONE

@ Return : NONE

@ Author : WZB

@  Date  : 2019 - 04 - 26
***************************************************************************************************************
*/
void xBSP_I2C_Simu_Start(void)
{
	SDA_OUT();     //sda线输出
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	xBSP_I2C_Simu_delay_us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	xBSP_I2C_Simu_delay_us(4);
	IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}	  

/*
***************************************************************************************************************
@ Name   : xBSP_I2C_Simu_Stop

@ Brief  : IIC 产生IIC停止信号

@ Param  : NONE

@ Return : NONE

@ Author : WZB

@  Date  : 2019 - 04 - 26
***************************************************************************************************************
*/
void xBSP_I2C_Simu_Stop(void)
{
	SDA_OUT();//sda线输出
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	xBSP_I2C_Simu_delay_us(4);
	IIC_SCL=1; 
	IIC_SDA=1;//发送I2C总线结束信号
	xBSP_I2C_Simu_delay_us(4);							   	
}

/*
***************************************************************************************************************
@ Name   : xBSP_I2C_Simu_Wait_Ack

@ Brief  : IIC 等待应答信号到来

@ Param  : NONE

@ Return : 0-接收应答成功，1-接收应答失败

@ Author : WZB

@  Date  : 2019 - 04 - 26
***************************************************************************************************************
*/
uint8_t xBSP_I2C_Simu_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	SDA_IN();
	IIC_SDA=1;xBSP_I2C_Simu_delay_us(1);	   
	IIC_SCL=1;xBSP_I2C_Simu_delay_us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			xBSP_I2C_Simu_Stop();
			return 1;
		}
	}
	IIC_SCL=0;	   
	return 0;  
} 

/*
***************************************************************************************************************
@ Name   : xBSP_I2C_Simu_Ack

@ Brief  : IIC 产生ACK应答

@ Param  : NONE

@ Return : NONE

@ Author : WZB

@  Date  : 2019 - 04 - 26
***************************************************************************************************************
*/
void xBSP_I2C_Simu_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	xBSP_I2C_Simu_delay_us(2);
	IIC_SCL=1;
	xBSP_I2C_Simu_delay_us(2);
	IIC_SCL=0;
}

/*
***************************************************************************************************************
@ Name   : xBSP_I2C_Simu_NAck

@ Brief  : IIC 不产生ACK应答	

@ Param  : NONE

@ Return : NONE

@ Author : WZB

@  Date  : 2019 - 04 - 26
***************************************************************************************************************
*/
void xBSP_I2C_Simu_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	xBSP_I2C_Simu_delay_us(2);
	IIC_SCL=1;
	xBSP_I2C_Simu_delay_us(2);
	IIC_SCL=0;
}					 				     

/*
***************************************************************************************************************
@ Name   : xBSP_I2C_Simu_Send_Byte

@ Brief  : IIC 发送一个字节	

@ Param  : uint8_t txd:数据

@ Return : NONE

@ Author : WZB

@  Date  : 2019 - 04 - 26
***************************************************************************************************************
*/
void xBSP_I2C_Simu_Send_Byte(uint8_t txd)
{
    uint8_t t;   
	SDA_OUT(); 	    
    IIC_SCL=0;
    for(t=0;t<8;t++)
    {
        IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		xBSP_I2C_Simu_delay_us(2);   //对TEA5767这三个延时都是必须的
		IIC_SCL=1;
		xBSP_I2C_Simu_delay_us(2); 
		IIC_SCL=0;	
		xBSP_I2C_Simu_delay_us(2);
    }	 
} 	    

/*
***************************************************************************************************************
@ Name   : xBSP_I2C_Simu_Read_Byte

@ Brief  : IIC 读1个字节数据

@ Param  : ack=1时，发送ACK，ack=0，发送nACK  

@ Return : 读取到的数据

@ Author : WZB

@  Date  : 2019 - 04 - 26
***************************************************************************************************************
*/
uint8_t xBSP_I2C_Simu_Read_Byte(uint8_t ack)
{
	unsigned char i,receive=0;
	SDA_IN();
    for(i=0;i<8;i++ )
	{
        IIC_SCL=0; 
        xBSP_I2C_Simu_delay_us(2);
		IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;   
		xBSP_I2C_Simu_delay_us(1); 
    }					 
    if (!ack)
        xBSP_I2C_Simu_NAck();
    else
        xBSP_I2C_Simu_Ack();
	IIC_SCL=0;
    return receive;
}


/*
***************************************************************************************************************
@ Name   : xBSP_I2C_Simu_WriteOneByte

@ Brief  : IIC 指定地址写入一字节数据

@ Param  : uint8_t devAddr：设备地址
@          uint8_t reg：写入数据的寄存器地址
@          uint8_t value：要写入的数据

@ Return : 写入结果：0-写入成功，1-写入失败

@ Author : WZB

@  Date  : 2019 - 04 - 26
***************************************************************************************************************
*/
uint8_t xBSP_I2C_Simu_WriteOneByte(uint8_t devAddr, uint8_t reg, uint8_t value)
{
    uint8_t ackFlag=0;
	xBSP_I2C_Simu_Start();

	xBSP_I2C_Simu_Send_Byte(devAddr&(~0x01));
	if(xBSP_I2C_Simu_Wait_Ack())  ackFlag = 1;
    xBSP_I2C_Simu_Send_Byte(reg);
	if(xBSP_I2C_Simu_Wait_Ack())  ackFlag = 1;
	xBSP_I2C_Simu_Send_Byte(value);						   
	if(xBSP_I2C_Simu_Wait_Ack())  ackFlag = 1;

    xBSP_I2C_Simu_Stop();
	
	return ackFlag;
}

/*
***************************************************************************************************************
@ Name   : xBSP_I2C_Simu_ReadOneByte

@ Brief  : IIC 指定地址读取一字节数据

@ Param  : uint8_t devAddr：设备地址
@          uint8_t reg：写入数据的寄存器地址

@ Return : 读取到的数据

@ Author : WZB

@  Date  : 2019 - 04 - 26
***************************************************************************************************************
*/
uint8_t xBSP_I2C_Simu_ReadOneByte(uint8_t devAddr, uint8_t reg)
{
	uint8_t rb = 0;

	xBSP_I2C_Simu_Start();  	 	   

    xBSP_I2C_Simu_Send_Byte(devAddr|0x01);
	xBSP_I2C_Simu_Wait_Ack();	    
	xBSP_I2C_Simu_Send_Byte(reg);     	   
	xBSP_I2C_Simu_Wait_Ack();	 
    rb=xBSP_I2C_Simu_Read_Byte(0);		   
	xBSP_I2C_Simu_Wait_Ack();  		    	   

    xBSP_I2C_Simu_Stop();	    
	return rb ;
}
