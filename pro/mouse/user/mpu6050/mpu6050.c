/***************************************************************/
#include "stm32f10x.h"
#include "mpu6050.h"
#include "24cxx.h"
#include "usart.h"
#include "stm32f10x_i2c.h"
/***************************************************************/
int mpu6050_buffer[3];//原始数据
int GYRO_XOFFSET=-29,GYRO_YOFFSET=-16,GYRO_ZOFFSET=-15;
#define FILTER_NUM 20
int16_t	GYRO_X_BUF[FILTER_NUM],GYRO_Y_BUF[FILTER_NUM],GYRO_Z_BUF[FILTER_NUM];
int GYRO_AVG_X,GYRO_AVG_Y,GYRO_AVG_Z,X24,Y24;//滤波后的数据
//**************************************
//初始化MPU6050
//**************************************
void mpu6050_Configuration(void)
{
	IIC_Configuration();
	Single_WriteI2C(PWR_MGMT_1, 0x00);	//解除休眠状态
	Single_WriteI2C(SMPLRT_DIV, 0x07);
	Single_WriteI2C(CONFIG, 0x06);
	Single_WriteI2C(GYRO_CONFIG, 0x18);
	Single_WriteI2C(ACCEL_CONFIG, 0x01);
}



//**************************************
//合成数据
//**************************************
int GetData(u8 REG_Address)
{
	u32 H;
	u32 L;
	H=Single_ReadI2C(REG_Address);
	L=Single_ReadI2C(REG_Address+1);
	return (H<<8)+L;   //合成数据
}
void mpu6050_data()
{
	mpu6050_buffer[0]=GetData(GYRO_XOUT_H)-GYRO_XOFFSET;
	mpu6050_buffer[1]=GetData(GYRO_YOUT_H)-GYRO_YOFFSET;
	mpu6050_buffer[2]=GetData(GYRO_ZOUT_H)-GYRO_ZOFFSET;
}

void analy_data()   //滑动窗口滤波
{
	static uint8_t filter_cnt=0;
	int32_t temp1=0,temp2=0,temp3=0;
	uint8_t i;
	
	GYRO_X_BUF[filter_cnt] = mpu6050_buffer[0];
	GYRO_Y_BUF[filter_cnt] = mpu6050_buffer[1];
	GYRO_Z_BUF[filter_cnt] = mpu6050_buffer[2];
	for(i=0;i<FILTER_NUM;i++)
	{
		temp1 += GYRO_X_BUF[i];
		temp2 += GYRO_Y_BUF[i];
		temp3 += GYRO_Z_BUF[i];
	}
	GYRO_AVG_X = temp1 / FILTER_NUM;
	GYRO_AVG_Y = temp2 / FILTER_NUM;
	GYRO_AVG_Z = temp3 / FILTER_NUM;
	filter_cnt++;
	if(filter_cnt==FILTER_NUM)	filter_cnt=0;
	
}

