/***************************************************************/
#include "stm32f10x.h"
#include "gpio.h"
#include "delay.h"
/***************************************************************/
#ifndef __IIC_H
#define __IIC_H 
/***************************************************************/


//IO方向设置
#define SCL_OUT() PB6_OUT  //注意地址
#define SDA_IN()  PB7_In
#define SDA_OUT() PB7_OUT

//IO操作函数	 
#define IIC_SCL    PBout(6) //SCL
#define IIC_SDA    PBout(7) //SDA	 
#define READ_SDA   PBin(7)  //输入SDA 

void IIC_Configuration(void);       //初始化IIC的IO口				 
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号
  

u8 Single_ReadI2C(u8 REG_Address);
void Single_WriteI2C(u8 REG_Address,u8 REG_data);




#endif




