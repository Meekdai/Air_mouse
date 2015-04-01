/***************************************************************/
#include "stm32f10x.h"
#include "spi.h"
/***************************************************************/



void SPI1_Configuration(void)
{	 
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
    
	// Enable SPI and GPIO clocks
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 ,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	/* Configure SPI1 pins: SCK, MISO and MOSI */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//GPIO_SetBits(GPIO_SPI1,SPI1_MISO| SPI1_MOSI| SPI1_SCK);
	/* Configure PB.12 as Output push-pull, used as Flash Chip select */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB, GPIO_Pin_7);
	/* SPI1 configuration */                                            //初始化SPI结构体
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //SPI设置为两线全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		                //设置SPI为主模式
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		            //SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		                    //串行时钟在不操作时，时钟为低电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	                    //第一个时钟沿开始采样数据
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		                    //NSS信号由软件（使用SSI位）管理
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;  //SPI波特率预分频值为8
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	                //数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	                        //CRC值计算的多项式
	SPI_Init(SPI1, &SPI_InitStructure);   //根据SPI_InitStruct中指定的参数初始化外设SPI2寄存器
	/* Enable SPI1  */
	SPI_Cmd(SPI1, ENABLE);                                      //使能SPI外设	
//	SPI1_ReadWriteByte(0xff);                                            //启动传输		
	GPIO_SetBits(GPIOB, GPIO_Pin_7);
}  

u8 SPI1_ReadWriteByte(u8 TxData)                                        //SPI读写数据函数
{		
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI1, TxData);
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	return SPI_I2S_ReceiveData(SPI1);			    
}



void SPI2_Configuration(void)
{	 
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
    
	// Enable SPI and GPIO clocks
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2 ,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	/* Configure SPI2 pins: SCK, MISO and MOSI */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//GPIO_SetBits(GPIO_SPI2,SPI2_MISO| SPI2_MOSI| SPI2_SCK);

	/* SPI2 configuration */                                            //初始化SPI结构体
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //SPI设置为两线全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		                //设置SPI为主模式
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		            //SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		                    //串行时钟在不操作时，时钟为低电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	                    //第一个时钟沿开始采样数据
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		                    //NSS信号由软件（使用SSI位）管理
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;  //SPI波特率预分频值为8
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	                //数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	                        //CRC值计算的多项式
	SPI_Init(SPI2, &SPI_InitStructure);   //根据SPI_InitStruct中指定的参数初始化外设SPI2寄存器
	/* Enable SPI2  */
	SPI_Cmd(SPI2, ENABLE);                                      //使能SPI外设	
	SPI2_ReadWriteByte(0xff);                                            //启动传输		 
}  

u8 SPI2_ReadWriteByte(u8 TxData)                                        //SPI读写数据函数
{		
		u8 retry=0;				 	
		while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET)      //发送缓存标志位为空
		{
				retry++;
				if(retry>200)return 0;
		}			  
		SPI_I2S_SendData(SPI2, TxData);                                    //通过外设SPI1发送一个数据
		retry=0;
		while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)   //接收缓存标志位不为空
		{
				retry++;
				if(retry>200)return 0;
		}	  						    
		return SPI_I2S_ReceiveData(SPI2);                                 //通过SPI1返回接收数据				    
}



void SPI3_Configuration(void)
{	 
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
    
	// Enable SPI and GPIO clocks
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3 ,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	/* Configure SPI3 pins: SCK, MISO and MOSI */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//GPIO_SetBits(GPIO_SPI2,SPI2_MISO| SPI2_MOSI| SPI2_SCK);

	/* SPI3 configuration */                                            //初始化SPI结构体
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //SPI设置为两线全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		                //设置SPI为主模式
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		            //SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		                    //串行时钟在不操作时，时钟为低电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	                    //第一个时钟沿开始采样数据
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		                    //NSS信号由软件（使用SSI位）管理
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;  //SPI波特率预分频值为8
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	                //数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	                        //CRC值计算的多项式
	SPI_Init(SPI3, &SPI_InitStructure);   //根据SPI_InitStruct中指定的参数初始化外设SPI2寄存器
	/* Enable SPI3  */
	SPI_Cmd(SPI3, ENABLE);                                      //使能SPI外设	
	SPI3_ReadWriteByte(0xff);                                            //启动传输		 
}  

u8 SPI3_ReadWriteByte(u8 TxData)                                        //SPI读写数据函数
{		
		u8 retry=0;				 	
		while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET)      //发送缓存标志位为空
		{
				retry++;
				if(retry>200)return 0;
		}			  
		SPI_I2S_SendData(SPI3, TxData);                                    //通过外设SPI1发送一个数据
		retry=0;
		while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET)   //接收缓存标志位不为空
		{
				retry++;
				if(retry>200)return 0;
		}	  						    
		return SPI_I2S_ReceiveData(SPI3);                                 //通过SPI1返回接收数据				    
}


