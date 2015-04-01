/***************************************************************/
#include "stm32f10x_it.h"
#include "gpio.h"
#include "delay.h"
#include "usart.h"
#include "mpu6050.h"
#include "nrf24l01.h"
/***************************************************************/
extern u8 USART_RX[60];
extern u8 USART_mark;
extern int USART_control;

extern int mpu6050_buffer[3];
extern int GYRO_AVG_X,GYRO_AVG_Y,GYRO_AVG_Z,X24,Y24;
extern u8  RXbag[16];
extern u8  TXbag[16];
/***************************************************************/
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)     //Check whether receiving interrupt
	{
		USART_RX[USART_mark++] = USART_ReceiveData(USART1);   // Read one byte from the receive data register
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		if(USART_mark ==60){USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);}			
	}
}

/***************************************************************/
void TIM3_IRQHandler(void)
{
	short x,y,z,i;
//	char temp[16]; 
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{

		mpu6050_data();
		analy_data();
		x=GYRO_AVG_X;
		y=GYRO_AVG_Y;
		z=GYRO_AVG_Z;
		
		GYRO_AVG_X=GYRO_AVG_X/30;
		GYRO_AVG_Y=GYRO_AVG_Y/30;
		GYRO_AVG_Z=GYRO_AVG_Z/30;
	
		X24=-GYRO_AVG_Z;
		Y24=GYRO_AVG_Y;

//		TXbag[0]=X24/100;
//		TXbag[1]=X24%100;
//		TXbag[2]=Y24/100;
//		TXbag[3]=Y24%100;
//		
//		if(NRF24L01_TxPacket(TXbag)==TX_OK)
//		{			
//				for(i=0;i<15;i++){TXbag[i]=0;}
//		}
			
//		Print_Show((int)x , (int)y, (int)z);
//		MPU6050_Mouse();
//		printf("OKOKKO");
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}


