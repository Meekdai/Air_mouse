/***************************************************************/
//2014.3.27  Copyright@ rhyme呆瓜云
/***************************************************************/
#include "stm32f10x.h"
#include "stdio.h"
#include "gpio.h"
#include "usart.h"
#include "delay.h"

#include "usb_lib.h"
#include "hw_config.h"
#include "usb_pwr.h"

//#include "mpu6050.h"
#include "tim.h"
#include "nrf24l01.h"
/***************************************************************/
u8 USART_RX[60]={""};
u8 USART_mark=0;
u8 Press_mark=0;
int USART_control=0;
int USART_data=0;

u8  TXbag[16]={""};
u8  RXbag[16]={""};
int GYRO_AVG_X,GYRO_AVG_Y,GYRO_AVG_Z,X24,Y24;
/***************************************************************/

int main(void)
{
	int i=0;
	RCC_Configuration();
	delay_Configuration(72);
	USART1_Configuration(115200);
	NRF24L01_Configuration();
	PC13_OUT
	Set_C13
	delay_ms(500);
	
	Set_System();
	USB_Interrupts_Config();
  Set_USBClock();
  USB_Init();
	USB_Cable_Config(ENABLE);               // 开启USB连接
	USB_Cable_Config(DISABLE);              // USB连接断开
	
	
	
	while(NRF24L01_Check())//检测不到24L01
	{
			delay_ms(400);
			Clr_C13
	}
	
	Set_C13

	RX_Mode();
	while(1)
	{
				Set_C13
				if(NRF24L01_RxPacket(RXbag)==0)//一旦接收到信息,则显示出来.
				{					
						Clr_C13
						X24=RXbag[0]*100+RXbag[1];
						Y24=RXbag[2]*100+RXbag[3];
						MPU6050_Mouse();
						for(i=0;i<15;i++){RXbag[i]=0;}
	//					printf(" X=%d  ,  Y=%d \r\n",X24,Y24);
				}	
	}
	
}
/***************************************************************/

