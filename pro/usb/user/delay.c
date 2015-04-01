/***************************************************************/
#include "stm32f10x.h"
#include "delay.h"
/***************************************************************/

static u8  fac_us=0;                                               // us The multiplier delay times 
static u16 fac_ms=0;                                               // ms The multiplier delay times 
/***************************************************************/
void delay_Configuration(u8 SYSCLK)
{
	SysTick->CTRL&=0xfffffffb;                                     // Select the internal clock HCLK/8		
	fac_us=SYSCLK/8;		    						   
	fac_ms=(u16)fac_us*1000;							 
}	
/***************************************************************/
void delay_ms(u16 nms)
{
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;                                 // Loading time
	SysTick->VAL =0x00;                                            // Empty the counter
	SysTick->CTRL=0x01 ;                                           // Start from bottom
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));                             // Wait time arrive
	SysTick->CTRL=0x00;                                            // Close the counter
	SysTick->VAL =0X00;                                            // Empty the counter	    
}		
/***************************************************************/
void delay_us(u32 Nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=Nus*fac_us;   		 
	SysTick->VAL=0x00;       
	SysTick->CTRL=0x01 ;       
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));
	SysTick->CTRL=0x00;       
	SysTick->VAL =0X00;       
}
/***************************************************************/
void delay_s(u32 Ns)
{		
	switch (Ns)
	{
		case 10: delay_ms(1000);
		case 9: delay_ms(1000);
		case 8: delay_ms(1000);
		case 7: delay_ms(1000);
		case 6: delay_ms(1000);
		case 5: delay_ms(1000);
		case 4: delay_ms(1000);
		case 3: delay_ms(1000);
		case 2: delay_ms(1000);
		case 1: delay_ms(1000);break;
	}
}


/***************************************************************/
void RCC_Configuration(void)
{
	ErrorStatus HSEStartUpStatus;								//ԭ���Ƕ�����ȫ�ֱ�����
    RCC_DeInit();									    		//ʱ�ӿ��ƼĴ���ȫ���ָ�Ĭ��ֵ
    RCC_HSEConfig(RCC_HSE_ON);						        	//�ⲿ����ʱ��Դ������8M����
    HSEStartUpStatus = RCC_WaitForHSEStartUp();					//�ȴ��ⲿʱ�Ӿ���
    if(HSEStartUpStatus == SUCCESS)								//���ʱ�������ɹ�
    {
		RCC_HCLKConfig(RCC_SYSCLK_Div1);						//����AHB�豸ʱ��Ϊϵͳʱ��1��Ƶ
    	RCC_PCLK2Config(RCC_HCLK_Div1);							//����AHB2�豸ʱ��ΪHCLKʱ��1��Ƶ
    	RCC_PCLK1Config(RCC_HCLK_Div2);							//����AHB1�豸ʱ��ΪHCLKʱ��2��Ƶ
		RCC_ADCCLKConfig(RCC_PCLK2_Div6);
    	FLASH_SetLatency(FLASH_Latency_2);					  	//�趨�ڲ�FLASH�ĵ���ʱ����Ϊ2����
    	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);	//ʹ��FLASHԤ��ȡ������
    	RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);	//����PLLʱ��Ϊ�ⲿ����ʱ�ӵ�9��Ƶ��8MHz * 9 = 72 MHz
    	RCC_PLLCmd(ENABLE);										//ʹ��PLLʱ��
    	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET){}    //�ȴ�PLLʱ���������׼������
    	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);				//ʹ��PLLʱ����Ϊϵͳʱ��Դ
    	while(RCC_GetSYSCLKSource() != 0x08){}					//����ϵͳ����ʱ��Դȷ��Ϊ�ⲿ���پ���8M����
  	} 
// 	//�ͷ�JTAGռ�õ�A13 A14 A15 B3 B4	  A14�������� A13��������
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
// 	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
			 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE); 
//   RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);		//��FSMCʱ��
}
/***************************************************************/
void SoftReset(void) 
{ 
	__set_FAULTMASK(1); 
	NVIC_SystemReset();
}

/***************************************************************/
