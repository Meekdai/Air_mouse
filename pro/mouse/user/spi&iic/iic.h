/***************************************************************/
#include "stm32f10x.h"
#include "gpio.h"
#include "delay.h"
/***************************************************************/
#ifndef __IIC_H
#define __IIC_H 
/***************************************************************/


//IO��������
#define SCL_OUT() PB6_OUT  //ע���ַ
#define SDA_IN()  PB7_In
#define SDA_OUT() PB7_OUT

//IO��������	 
#define IIC_SCL    PBout(6) //SCL
#define IIC_SDA    PBout(7) //SDA	 
#define READ_SDA   PBin(7)  //����SDA 

void IIC_Configuration(void);       //��ʼ��IIC��IO��				 
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�
  

u8 Single_ReadI2C(u8 REG_Address);
void Single_WriteI2C(u8 REG_Address,u8 REG_data);




#endif




