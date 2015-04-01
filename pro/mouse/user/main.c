/***************************************************************/
//2014.3.27  Copyright@ rhyme呆瓜云
/***************************************************************/
#include "stm32f10x.h"
#include "stdio.h"
#include "gpio.h"
#include "usart.h"
#include "delay.h"
#include "mpu6050.h"
#include "tim.h"
#include "nrf24l01.h"
#include "adc.h"
/***************************************************************/
u8 USART_RX[60]={""};
u8 USART_mark=0;
u8 Press_mark=0;
int USART_control=0;
int USART_data=0;

u8  TXbag[16]={""};
u8  RXbag[16]={""};
extern int GYRO_AVG_X,GYRO_AVG_Y,GYRO_AVG_Z,X24,Y24;

extern u8  TIM2CH1_CAPTURE_STA;		//输入捕获状态		    				
extern u16	TIM2CH1_CAPTURE_VAL;	//输入捕获值	
/***************************************************************/


unsigned char Trg;
unsigned char Cont;
void KeyRead( void )
{
    unsigned char ReadData = A4;
    Trg = ReadData & (ReadData ^ Cont);
    Cont = ReadData;
}

int main(void)
{
	int i=0;
	u32 temp=0;
	RCC_Configuration();
	delay_Configuration(72);
	USART1_Configuration(9600);
	PB8_OUT
	Set_B8
	PA3_In
	PA4_In
	delay_ms(500);
	mpu6050_Configuration(); 
	NRF24L01_Configuration();
	while(NRF24L01_Check())//检测不到24L01
	{
			delay_ms(400);
			Set_B8
	}
	
	Clr_B8
	TX_Mode();
	
	ADC_Configuration(ADC1,9,55.5);



	TIM_Configuration(TIM3,10,7200,1);  //1ms
	while(1)
	{
		if(Get_ADC_Voltage( ADC1)<1800){Set_B8}
		
		TXbag[0]=X24/100;
		TXbag[1]=X24%100;
		TXbag[2]=Y24/100;
		TXbag[3]=Y24%100;
		
		KeyRead();
		if(Trg==1){TXbag[4]=5;}
		else if(Trg==0&&Cont==1){TXbag[4]=10;}
		else if(Trg==0&&Cont==0){TXbag[4]=0;}
		
		if(A3==1){TXbag[5]=5;}else {TXbag[5]=0;}

		
		if(NRF24L01_TxPacket(TXbag)==TX_OK)
		{			
				for(i=0;i<15;i++){TXbag[i]=0;}
		}
	}
	
}
/***************************************************************/

