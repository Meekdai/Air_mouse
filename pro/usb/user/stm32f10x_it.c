/***************************************************************/
#include "stm32f10x_it.h"
#include "gpio.h"
#include "delay.h"
#include "usart.h"

#include "usb_istr.h"
#include "usb_lib.h"
#include "usb_pwr.h"
#include "platform_config.h"
#include "hw_config.h"
/***************************************************************/
extern u8 USART_RX[60];
extern u8 USART_mark;
extern int USART_control;

extern int GYRO_AVG_X,GYRO_AVG_Y,GYRO_AVG_Z,X24,Y24;
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

void USB_LP_CAN1_RX0_IRQHandler(void)
{
  USB_Istr();
}
/***************************************************************/