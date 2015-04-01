/***************************************************************/
#include "stm32f10x.h"
#include "iic.h"
#include "mpu6050.h"
/***************************************************************/



 
//��ʼ��IIC
void IIC_Configuration(void)
{					     
	SCL_OUT()
	SDA_OUT()
}
//����IIC��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT();     //sda�����
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(5);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(5);
	IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void IIC_Stop(void)
{
	SDA_OUT();//sda�����
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(5);
	IIC_SCL=1; 
	IIC_SDA=1;//����I2C���߽����ź�
	delay_us(5);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
	IIC_SDA=1;delay_us(1);	   
	IIC_SCL=1;delay_us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}
//������ACKӦ��		    
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        IIC_SDA=(txd&0x80)>>7;
//		if((txd&0x80)>>7)
//			IIC_SDA=1;
//		else
//			IIC_SDA=0;
		txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		IIC_SCL=1;
		delay_us(2); 
		IIC_SCL=0;	
		delay_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        IIC_SCL=0; 
        delay_us(2);
				IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;   
				delay_us(1); 
    }					 
    if (!ack)
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK   
    return receive;
}

//**************************************
//��I2C�豸д��һ���ֽ�����
//**************************************
void Single_WriteI2C(u8 REG_Address,u8 REG_data)
{
    IIC_Start();                  //��ʼ�ź�
    IIC_Send_Byte(SlaveAddress);   //�����豸��ַ+д�ź�
		IIC_Wait_Ack();
    IIC_Send_Byte(REG_Address);    //�ڲ��Ĵ�����ַ
		IIC_Wait_Ack();
    IIC_Send_Byte(REG_data);       //�ڲ��Ĵ�������
		IIC_Wait_Ack();
    IIC_Stop();                   //����ֹͣ�ź�
}



//**************************************
//��I2C�豸��ȡһ���ֽ�����
//**************************************
u8 Single_ReadI2C(u8 REG_Address)
{
	u8 REG_data;
	IIC_Start();                   //��ʼ�ź�
	IIC_Send_Byte(SlaveAddress);    //�����豸��ַ+д�ź�
	IIC_Wait_Ack();
	IIC_Send_Byte(REG_Address);     //���ʹ洢��Ԫ��ַ����0��ʼ	
	IIC_Wait_Ack();
	IIC_Start();                   //��ʼ�ź�
	IIC_Send_Byte(SlaveAddress+1);  //�����豸��ַ+���ź�
	IIC_Wait_Ack();
	REG_data=IIC_Read_Byte(0);       //�����Ĵ�������
	//IIC_SendACK(1);                //����Ӧ���ź�
	IIC_Stop();                    //ֹͣ�ź�
	return REG_data;
}

