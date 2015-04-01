/******************** (C) COPYRIGHT 2010 STMicroelectronics ********************
* File Name          : hw_config.c
* Author             : MCD Application Team
* Version            : V3.2.1
* Date               : 07/05/2010
* Description        : Hardware Configuration & Setup
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "hw_config.h"
#include "usb_desc.h"
#include "platform_config.h"
#include "usb_pwr.h"
#include "usb_lib.h"

#include "usart.h"
#include "mpu6050.h"
#include "gpio.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#define JOY_UP       GPIO_Pin_4  /* PD.4 */
#define JOY_DOWN     GPIO_Pin_1  /* PD.1 */
#define JOY_LEFT     GPIO_Pin_2  /* PD.2 */
#define JOY_RIGHT    GPIO_Pin_3  /* PD.3 */
#define JOY_LEFT_BUTTON    GPIO_Pin_13  /* PC.13 */
#define JOY_RIGHT_BUTTON    GPIO_Pin_8  /* PA.8 */

/* Extern variables ----------------------------------------------------------*/
ErrorStatus HSEStartUpStatus;
/* Private function prototypes -----------------------------------------------*/
static void IntToUnicode (uint32_t value , uint8_t *pbuf , uint8_t len);
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : Set_System
* Description    : Configures Main system clocks & power.
* Input          : None.
* Return         : None.
*******************************************************************************/
void Set_System(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

//  GPIO_AINConfig();

    /* Enable GPIOD and GPIOA clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIO_DISCONNECT, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);

  /* PB.06 used as USB pull-up */
  GPIO_InitStructure.GPIO_Pin = USB_DISCONNECT_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(USB_DISCONNECT, &GPIO_InitStructure);

  /* Configure the JoyStick IOs */
  /* Key up + Key down Key left + Key right */
//  GPIO_InitStructure.GPIO_Pin = JOY_UP | JOY_DOWN | JOY_LEFT | JOY_RIGHT;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//  GPIO_Init(GPIOD, &GPIO_InitStructure);

//  /* Left_button */
//  GPIO_InitStructure.GPIO_Pin = JOY_LEFT_BUTTON;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//  GPIO_Init(GPIOC, &GPIO_InitStructure);

//  /* Right_button */	   
//  GPIO_InitStructure.GPIO_Pin = JOY_RIGHT_BUTTON;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /**
  *  LED1 -> PD8 , LED2 -> PD9 , LED3 -> PD10 , LED4 -> PD11
  */			
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; ; 
//  GPIO_Init(GPIOD, &GPIO_InitStructure);

//  TIM_USB_Configuration();
//  NVIC_Configuration();
}

/*******************************************************************************
* Function Name  : TIM_Configuration
* Description    : TIM_Configuration program.
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void TIM_USB_Configuration(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
  TIM_DeInit(TIM2);
  TIM_TimeBaseStructure.TIM_Period=2000;		 					/* 自动重装载寄存器周期的值(计数值) */
																	/* 累计 TIM_Period个频率后产生一个更新或者中断 */
  TIM_TimeBaseStructure.TIM_Prescaler= (36000 - 1);				    /* 时钟预分频数   例如：时钟频率=72MHZ/(时钟预分频+1) */
  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 			/* 采样分频 */
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 		/* 向上计数模式 */
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  TIM_ClearFlag(TIM2, TIM_FLAG_Update);							    /* 清除溢出中断标志 */
  TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
  TIM_Cmd(TIM2, ENABLE);											/* 开启时钟 */
}

/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configuration the nested vectored interrupt controller.
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure; 
	 
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);  													
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	  
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
* Function Name  : Set_USBClock
* Description    : Configures USB Clock input (48MHz).
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Set_USBClock(void)
{
  /* Select USBCLK source */
  RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);

  /* Enable the USB clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);
}

/*******************************************************************************
* Function Name  : GPIO_AINConfig
* Description    : Configures all IOs as AIN to reduce the power consumption.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void GPIO_AINConfig(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable all GPIOs Clock*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ALLGPIO, ENABLE);

  /* Configure all GPIO port pins in Analog Input mode (floating input trigger OFF) */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  /* Disable all GPIOs Clock*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ALLGPIO, DISABLE);
}
/*******************************************************************************
* Function Name  : Enter_LowPowerMode.
* Description    : Power-off system clocks and power while entering suspend mode.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Enter_LowPowerMode(void)
{

}

/*******************************************************************************
* Function Name  : Leave_LowPowerMode.
* Description    : Restores system clocks and power while exiting suspend mode.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Leave_LowPowerMode(void)
{

}

/*******************************************************************************
* Function Name  : USB_Interrupts_Config.
* Description    : Configures the USB interrupts.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void USB_Interrupts_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* 2 bit for pre-emption priority, 2 bits for subpriority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

  /* Enable the USB interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
* Function Name  : USB_Cable_Config.
* Description    : Software Connection/Disconnection of USB Cable.
* Input          : NewState: new state.
* Output         : None.
* Return         : None
*******************************************************************************/
void USB_Cable_Config (FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
    GPIO_ResetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
  }
  else
  {
    GPIO_SetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
  }
}

/*******************************************************************************
* Function Name : JoyState.
* Description   : Decodes the Joystick direction.
* Input         : None.
* Output        : None.
* Return value  : The direction value.
*******************************************************************************/
extern int GYRO_AVG_X,GYRO_AVG_Y,GYRO_AVG_Z;
uint8_t JoyState(void)
{
  /* "right" key is pressed */
//  if (!GPIO_ReadInputDataBit(GPIOD, JOY_RIGHT))
//  {
//    return RIGHT;
//  }
//  /* "left" key is pressed */
//  if (!GPIO_ReadInputDataBit(GPIOD, JOY_LEFT))
//  {
//    return LEFT;
//  }
//  /* "up" key is pressed */
//  if (!GPIO_ReadInputDataBit(GPIOD, JOY_UP))
//  {
//    return UP;
//  }
//  /* "down" key is pressed */
//  if (!GPIO_ReadInputDataBit(GPIOD, JOY_DOWN))
//  {
//    return DOWN;
//  }
	
//  if ( !GPIO_ReadInputDataBit(GPIOC, JOY_LEFT_BUTTON))
//  {
//    return LEFT_BUTTON;
//  }
//   if ( !GPIO_ReadInputDataBit(GPIOA, JOY_RIGHT_BUTTON))
//  {
//    return RIGHT_BUTTON;
//  }
//	if(GYRO_AVG_Z>500){return LEFT;}
//	if(GYRO_AVG_Z<-500){return RIGHT;}
  /* No key is pressed */
//  else
//  {
    return 0;
//  }
}

/*******************************************************************************
* Function Name : Joystick_Send.
* Description   : prepares buffer to be sent containing Joystick event infos.
* Input         : Keys: keys received from terminal.
* Output        : None.
* Return value  : None.
*******************************************************************************/
void Joystick_Send(uint8_t Keys)
{
  uint8_t Mouse_Buffer[4] = {0, 0, 0, 0};
  int8_t X = 0, Y = 0,MouseButton=0;

  switch (Keys)
  {
    case LEFT:
      X -= CURSOR_STEP;
      break;

    case RIGHT:
      X += CURSOR_STEP;
      break;
	  
    case UP:
      Y -= CURSOR_STEP;
      break;

    case DOWN:
      Y += CURSOR_STEP;
	  break;
	   
	case LEFT_BUTTON:
      MouseButton = MouseButton|0x01;
      break;

 	case RIGHT_BUTTON:
      MouseButton = MouseButton|0x02;
      break;

    default:
      return;
  }

  /* prepare buffer to send */
  Mouse_Buffer[0] = MouseButton;
  Mouse_Buffer[1] = X;
  Mouse_Buffer[2] = Y;
  
  /* Copy mouse position info in ENDP1 Tx Packet Memory Area*/
  USB_SIL_Write(EP1_IN, Mouse_Buffer, 4);

  if(Mouse_Buffer[0]!= 0)
  {
    Mouse_Buffer[0] = 0;
    /* Copy mouse position info in ENDP1 Tx Packet Memory Area*/
    USB_SIL_Write(EP1_IN, Mouse_Buffer, 4);
  } 
  /* enable endpoint for transmission */
  SetEPTxValid(ENDP1);  
}
extern int GYRO_AVG_X,GYRO_AVG_Y,GYRO_AVG_Z,X24,Y24;
extern u8  RXbag[16];
void MPU6050_Mouse()
{
	uint8_t Mouse_Buffer[4] = {0, 0, 0, 0};
  int8_t X = 0, Y = 0,MouseButton=0;
	
	GYRO_AVG_X=GYRO_AVG_X/30;
	GYRO_AVG_Y=GYRO_AVG_Y/30;
	GYRO_AVG_Z=GYRO_AVG_Z/30;
	
	X=-GYRO_AVG_Z;
	Y=-GYRO_AVG_X;
	
  Mouse_Buffer[1] = X24;
  Mouse_Buffer[2] = Y24;	
	
  if (RXbag[4]==5){MouseButton = MouseButton|0x01;Mouse_Buffer[1] =Mouse_Buffer[2]= 0;}
	else if(RXbag[4]==10){MouseButton = MouseButton|0x01;}
	else {MouseButton = 0;}
	
	if (RXbag[5]==5){MouseButton = MouseButton|0x02;Mouse_Buffer[1] =Mouse_Buffer[2]= 0;}
//	else if(RXbag[5]==10){MouseButton = MouseButton|0x02;}
//	else {MouseButton = 0;}
	
  Mouse_Buffer[0] = MouseButton;
  USB_SIL_Write(EP1_IN, Mouse_Buffer, 4);

//  if(Mouse_Buffer[0]!= 0)
//  {
//    Mouse_Buffer[1] =Mouse_Buffer[2]= 0;
//    USB_SIL_Write(EP1_IN, Mouse_Buffer, 4);
//  } 
  SetEPTxValid(ENDP1); 
}
/*******************************************************************************
* Function Name  : Get_SerialNum.
* Description    : Create the serial number string descriptor.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Get_SerialNum(void)
{
  uint32_t Device_Serial0, Device_Serial1, Device_Serial2;

  Device_Serial0 = *(__IO uint32_t*)(0x1FFFF7E8);
  Device_Serial1 = *(__IO uint32_t*)(0x1FFFF7EC);
  Device_Serial2 = *(__IO uint32_t*)(0x1FFFF7F0);

  Device_Serial0 += Device_Serial2;

  if (Device_Serial0 != 0)
  {
    IntToUnicode (Device_Serial0, &Joystick_StringSerial[2] , 8);
    IntToUnicode (Device_Serial1, &Joystick_StringSerial[18], 4);
  }
}

/*******************************************************************************
* Function Name  : HexToChar.
* Description    : Convert Hex 32Bits value into char.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
static void IntToUnicode (uint32_t value , uint8_t *pbuf , uint8_t len)
{
  uint8_t idx = 0;
  
  for( idx = 0 ; idx < len ; idx ++)
  {
    if( ((value >> 28)) < 0xA )
    {
      pbuf[ 2* idx] = (value >> 28) + '0';
    }
    else
    {
      pbuf[2* idx] = (value >> 28) + 'A' - 10; 
    }
    
    value = value << 4;
    
    pbuf[ 2* idx + 1] = 0;
  }
}
/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
