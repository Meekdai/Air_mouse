/***************************************************************/
#include "stm32f10x.h"
#include "usart.h"
/***************************************************************/
u8 USARTx_MARK=0;
extern u8 USART_RX[60];
extern u8 USART_mark;
extern int USART_control;
/***************************************************************/
void USART1_Configuration(u32 BaudRate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQChannel;   //ͨ������Ϊ����1�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =10;   //�ж�ռ�ȵȼ�10
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;          //�ж���Ӧ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //���ж�
	NVIC_Init(&NVIC_InitStructure);                             //��ʼ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	/* ���� USART1 Tx (PA9)*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	 		//���ÿ�©���ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//������Ƶ��Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* ���� USART1 Rx (PA10)*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//��������ģʽ
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate            =BaudRate;	  //������
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b; //8λ����
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;	 //ֹͣλ1λ
	USART_InitStructure.USART_Parity              = USART_Parity_No ;	 //��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);
	USARTx_MARK=1;
	printf("\r\n USART1 Configuration OK\r\n");
}

void USART21_Configuration(u32 BaudRate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQChannel ;  //ͨ������Ϊ����2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =11;   //�ж�ռ�ȵȼ�11
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;          //�ж���Ӧ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //���ж�
	NVIC_Init(&NVIC_InitStructure);                             //��ʼ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	 		//���ÿ�©���ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//������Ƶ��Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* ���� USART2 Rx (PA3)*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//��������ģʽ
	GPIO_Init(GPIOA, &GPIO_InitStructure);  
		
	USART_InitStructure.USART_BaudRate            = BaudRate;	  //������
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b; //8λ����
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;	 //ֹͣλ1λ
	USART_InitStructure.USART_Parity              = USART_Parity_No ;	 //��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART2, ENABLE);
	USARTx_MARK=2;
	printf("\r\n USART21 Configuration OK\r\n");
}
void USART22_Configuration(u32 BaudRate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQChannel ;  //ͨ������Ϊ����2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =11;   //�ж�ռ�ȵȼ�11
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;          //�ж���Ӧ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //���ж�
	NVIC_Init(&NVIC_InitStructure);                             //��ʼ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);            //����USART ���IO
	/* ���� USART2Tx (PD5) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	 		//���ÿ�©���ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//������Ƶ��Ϊ50MHz
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* ���� USART2 Rx (PD6)*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//��������ģʽ
	GPIO_Init(GPIOD, &GPIO_InitStructure); 

	
	USART_InitStructure.USART_BaudRate            = BaudRate;	  //������
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b; //8λ����
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;	 //ֹͣλ1λ
	USART_InitStructure.USART_Parity              = USART_Parity_No ;	 //��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART2, ENABLE);
	USARTx_MARK=2;
	printf("\r\n USART22 Configuration OK\r\n");
}


void USART3_Configuration(u32 BaudRate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQChannel ;  //ͨ������Ϊ����3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =12;   //�ж�ռ�ȵȼ�12
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;          //�ж���Ӧ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //���ж�
	NVIC_Init(&NVIC_InitStructure);                             //��ʼ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1| RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	/* ���� USART3 Tx (PB10) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	 		//���ÿ�©���ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//������Ƶ��Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* ���� USART3 Rx (PB11) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//��������ģʽ
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	USART_InitStructure.USART_BaudRate            = BaudRate;	  //������
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b; //8λ����
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;	 //ֹͣλ1λ
	USART_InitStructure.USART_Parity              = USART_Parity_No ;	 //��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART3, ENABLE);
	USARTx_MARK=3;
	printf("\r\n USART3 Configuration OK\r\n");
}

void USART4_Configuration(u32 BaudRate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQChannel ;  //ͨ������Ϊ����4�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =13;   //�ж�ռ�ȵȼ�13
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;          //�ж���Ӧ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //���ж�
	NVIC_Init(&NVIC_InitStructure);                             //��ʼ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	/* ���� USART4 Tx (PC10) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	/* ���� USART4 Rx (PC11) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = BaudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART4, &USART_InitStructure);
	USART_Cmd(UART4, ENABLE);
	USARTx_MARK=4;
	printf("\r\n USART4 Configuration OK\r\n");
}


void USART5_Configuration(u32 BaudRate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQChannel ;  //ͨ������Ϊ����5�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =14;   //�ж�ռ�ȵȼ�14
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;          //�ж���Ӧ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //���ж�
	NVIC_Init(&NVIC_InitStructure);                             //��ʼ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
	/* ���� USART5 Tx (PC12) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	/* ���� USART5 Rx (PC2) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = BaudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART5, &USART_InitStructure);
	USART_Cmd(UART5, ENABLE);
	USARTx_MARK=5;
	printf("\r\n USART5 Configuration OK\r\n");
}


/***************************************************************/
int fputc(int ch, FILE *f)						  //����PRINTF�������ó���
{
	if(USARTx_MARK==1)
	{
		USART_SendData(USART1, (unsigned char) ch);
		while (!(USART1->SR & USART_FLAG_TXE));		 //�ȴ��������
	}
	else if(USARTx_MARK==2)
	{
		USART_SendData(USART2, (unsigned char) ch);
		while (!(USART2->SR & USART_FLAG_TXE));		 //�ȴ��������
	}
  else if(USARTx_MARK==3)
	{
		USART_SendData(USART3, (unsigned char) ch);
		while (!(USART3->SR & USART_FLAG_TXE));		 //�ȴ��������		
	}
	else if(USARTx_MARK==4)
	{
		USART_SendData(UART4, (unsigned char) ch);
		while (!(UART4->SR & USART_FLAG_TXE));		 //�ȴ��������
	}
  else if(USARTx_MARK==5)
	{
		USART_SendData(UART5, (unsigned char) ch);
		while (!(UART5->SR & USART_FLAG_TXE));		 //�ȴ��������		
	}
	return (ch);
}

/***************************************************************/

void Get_RX_4bitData(void)
{
			//USART_control=(USART_RX[0]-'0');
		 	USART_control=(USART_RX[0]-'0')*1000+(USART_RX[1]-'0')*100+(USART_RX[2]-'0')*10+(USART_RX[3]-'0');
 			USART_mark=0;
}
/*************************����16������**********************************************/
void Send_Hex(u8 USART,u8 c)         
{
    if(USART==1)
	{
	    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	  //������Ϊ��
        USART_SendData(USART1, c);
	    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	 //�������
	}
	else if(USART==2)
	{
	    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);	  //������Ϊ��
        USART_SendData(USART2, c);
	    while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);	 //�������
	}
	else if(USART==3)
	{
	    while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);	  //������Ϊ��
        USART_SendData(USART3, c);
	    while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);	 //�������
	}
	else if(USART==4)
	{
	    while(USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET);	  //������Ϊ��
        USART_SendData(UART4, c);
	    while (USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET);	 //�������
	}
	else if(USART==5)
	{
	    while(USART_GetFlagStatus(UART5, USART_FLAG_TXE) == RESET);	  //������Ϊ��
        USART_SendData(UART5, c);
	    while (USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET);	 //�������
	}
}


void SeriPush2Bytes(int value)
{
	unsigned char t_char;
	if(value < 0) {
		value = -value;
		Send_Hex(USARTx_MARK,0xf0);
	}
	else {
		Send_Hex(USARTx_MARK,0xf5);
	}
	t_char = value/256;
	if(t_char > 127)	Send_Hex(USARTx_MARK,0x80);
	else	Send_Hex(USARTx_MARK,0x00);
	Send_Hex(USARTx_MARK,t_char & 0x7f);
	t_char = value%256;
	if(t_char > 127)	Send_Hex(USARTx_MARK,0x80);
	else	Send_Hex(USARTx_MARK,0x00);
	Send_Hex(USARTx_MARK,t_char & 0x7f);
}

void Print_Show(int data1, int data2, int data3)
{
	SeriPush2Bytes(data1);	     
	SeriPush2Bytes(data2);	     
	SeriPush2Bytes(data3);	     
	Send_Hex(USARTx_MARK,0xff);
}
