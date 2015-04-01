
/***************************************************************/
#include "stm32f10x.h"
#include "nrf24l01.h"
#include "spi.h"
/***************************************************************/

const u8 TX_ADDRESS[TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //���͵�ַ
const u8 RX_ADDRESS[RX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //���͵�ַ	



void SPI_Configuration()
{
		SPI2_Configuration();  
}
u8 SPI_ReadWriteByte(u8 TxData)
{
		return SPI2_ReadWriteByte(TxData);
}



//��ʼ��24L01��IO��
void NRF24L01_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_NRF24L01_CE, ENABLE);    //ʹ��GPIO��ʱ��
	GPIO_InitStructure.GPIO_Pin = NRF24L01_CE;          //NRF24L01 ģ��Ƭѡ�ź�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_NRF24L01_CE, &GPIO_InitStructure);

	RCC_APB2PeriphClockCmd(RCC_NRF24L01_CSN, ENABLE);   //ʹ��GPIO��ʱ��
	GPIO_InitStructure.GPIO_Pin = NRF24L01_CSN;      
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_NRF24L01_CSN, &GPIO_InitStructure);
	
	Set_NRF24L01_CE;                                    //��ʼ��ʱ������
	Set_NRF24L01_CSN;                                   //��ʼ��ʱ������

    //����NRF2401��IRQ
	GPIO_InitStructure.GPIO_Pin = NRF24L01_IRQ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU  ;     //��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_NRF24L01_IRQ, &GPIO_InitStructure);
	GPIO_SetBits(GPIO_NRF24L01_IRQ,NRF24L01_IRQ);

	SPI_Configuration();                                //��ʼ��SPI
	Clr_NRF24L01_CE; 	                                //ʹ��24L01
	Set_NRF24L01_CSN;                                   //SPIƬѡȡ��
}
//�ϵ���NRF24L01�Ƿ���λ
//д5������Ȼ���ٶ��������бȽϣ�
//��ͬʱ����ֵ:0����ʾ��λ;���򷵻�1����ʾ����λ	
u8 NRF24L01_Check(void)
{
	u8 buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	u8 buf1[5];
	u8 i;   	 
	NRF24L01_Write_Buf(SPI_WRITE_REG+TX_ADDR,buf,5);//д��5���ֽڵĵ�ַ.	
	NRF24L01_Read_Buf(TX_ADDR,buf1,5);              //����д��ĵ�ַ  	
	for(i=0;i<5;i++)if(buf1[i]!=0XA5)break;					   
	if(i!=5)return 1;                               //NRF24L01����λ	
	return 0;		                                //NRF24L01��λ
}	 	 
//ͨ��SPIд�Ĵ���
u8 NRF24L01_Write_Reg(u8 regaddr,u8 data)
{
	u8 status;	
    Clr_NRF24L01_CSN;                    //ʹ��SPI����
  	status =SPI_ReadWriteByte(regaddr); //���ͼĴ����� 
  	SPI_ReadWriteByte(data);            //д��Ĵ�����ֵ
  	Set_NRF24L01_CSN;                    //��ֹSPI����	   
  	return(status);       		         //����״ֵ̬
}
//��ȡSPI�Ĵ���ֵ ��regaddr:Ҫ���ļĴ���
u8 NRF24L01_Read_Reg(u8 regaddr)
{
	u8 reg_val;	    
	Clr_NRF24L01_CSN;                //ʹ��SPI����		
  	SPI_ReadWriteByte(regaddr);     //���ͼĴ�����
  	reg_val=SPI_ReadWriteByte(0XFF);//��ȡ�Ĵ�������
  	Set_NRF24L01_CSN;                //��ֹSPI����		    
  	return(reg_val);                 //����״ֵ̬
}	
//��ָ��λ�ö���ָ�����ȵ�����
//*pBuf:����ָ��
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ 
u8 NRF24L01_Read_Buf(u8 regaddr,u8 *pBuf,u8 datalen)
{
	u8 status,u8_ctr;	       
  	Clr_NRF24L01_CSN;                     //ʹ��SPI����
  	status=SPI_ReadWriteByte(regaddr);   //���ͼĴ���ֵ(λ��),����ȡ״ֵ̬   	   
	for(u8_ctr=0;u8_ctr<datalen;u8_ctr++)pBuf[u8_ctr]=SPI_ReadWriteByte(0XFF);//��������
  	Set_NRF24L01_CSN;                     //�ر�SPI����
  	return status;                        //���ض�����״ֵ̬
}
//��ָ��λ��дָ�����ȵ�����
//*pBuf:����ָ��
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ
u8 NRF24L01_Write_Buf(u8 regaddr, u8 *pBuf, u8 datalen)
{
	u8 status,u8_ctr;	    
	Clr_NRF24L01_CSN;                                    //ʹ��SPI����
  	status = SPI_ReadWriteByte(regaddr);                //���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
  	for(u8_ctr=0; u8_ctr<datalen; u8_ctr++)SPI_ReadWriteByte(*pBuf++); //д������	 
  	Set_NRF24L01_CSN;                                    //�ر�SPI����
  	return status;                                       //���ض�����״ֵ̬
}				   
//����NRF24L01����һ������
//txbuf:�����������׵�ַ
//����ֵ:�������״��
u8 NRF24L01_TxPacket(u8 *txbuf)
{
	u8 state;   
	Clr_NRF24L01_CE;
	NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//д���ݵ�TX BUF  32���ֽ�
	Set_NRF24L01_CE;                                     //��������	   
	while(READ_NRF24L01_IRQ!=0);                         //�ȴ��������
	state=NRF24L01_Read_Reg(STATUS);                     //��ȡ״̬�Ĵ�����ֵ	   
	NRF24L01_Write_Reg(SPI_WRITE_REG+STATUS,state);      //���TX_DS��MAX_RT�жϱ�־
	if(state&MAX_TX)                                     //�ﵽ����ط�����
	{
		NRF24L01_Write_Reg(FLUSH_TX,0xff);               //���TX FIFO�Ĵ��� 
		return MAX_TX; 
	}
	if(state&TX_OK)                                      //�������
	{
		return TX_OK;
	}
	return 0xff;                                         //����ԭ����ʧ��
}

//����NRF24L01����һ������
//txbuf:�����������׵�ַ
//����ֵ:0��������ɣ��������������
u8 NRF24L01_RxPacket(u8 *rxbuf)
{
	u8 state;		    							      
	state=NRF24L01_Read_Reg(STATUS);                //��ȡ״̬�Ĵ�����ֵ    	 
	NRF24L01_Write_Reg(SPI_WRITE_REG+STATUS,state); //���TX_DS��MAX_RT�жϱ�־
	if(state&RX_OK)                                 //���յ�����
	{
		NRF24L01_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//��ȡ����
		NRF24L01_Write_Reg(FLUSH_RX,0xff);          //���RX FIFO�Ĵ��� 
		return 0; 
	}	   
	return 1;                                      //û�յ��κ�����
}

//�ú�����ʼ��NRF24L01��RXģʽ
//����RX��ַ,дRX���ݿ��,ѡ��RFƵ��,�����ʺ�LNA HCURR
//��CE��ߺ�,������RXģʽ,�����Խ���������		   
void RX_Mode(void)
{
	Clr_NRF24L01_CE;	  
    //дRX�ڵ��ַ
  	NRF24L01_Write_Buf(SPI_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH);

    //ʹ��ͨ��0���Զ�Ӧ��    
  	NRF24L01_Write_Reg(SPI_WRITE_REG+EN_AA,0x01);    
    //ʹ��ͨ��0�Ľ��յ�ַ  	 
  	NRF24L01_Write_Reg(SPI_WRITE_REG+EN_RXADDR,0x01);
    //����RFͨ��Ƶ��		  
  	NRF24L01_Write_Reg(SPI_WRITE_REG+RF_CH,40);	     
    //ѡ��ͨ��0����Ч���ݿ�� 	    
  	NRF24L01_Write_Reg(SPI_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);
    //����TX�������,0db����,2Mbps,���������濪��   
  	NRF24L01_Write_Reg(SPI_WRITE_REG+RF_SETUP,0x0f);
    //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,PRIM_RX����ģʽ 
  	NRF24L01_Write_Reg(SPI_WRITE_REG+NCONFIG, 0x0f); 
    //CEΪ��,�������ģʽ 
  	Set_NRF24L01_CE;                                
}			

//�ú�����ʼ��NRF24L01��TXģʽ
//����TX��ַ,дTX���ݿ��,����RX�Զ�Ӧ��ĵ�ַ,���TX��������,
//ѡ��RFƵ��,�����ʺ�LNA HCURR PWR_UP,CRCʹ��
//��CE��ߺ�,������RXģʽ,�����Խ���������		   
//CEΪ�ߴ���10us,����������.	 
void TX_Mode(void)
{														 
	Clr_NRF24L01_CE;	    
    //дTX�ڵ��ַ 
  	NRF24L01_Write_Buf(SPI_WRITE_REG+TX_ADDR,(u8*)TX_ADDRESS,TX_ADR_WIDTH);    
    //����TX�ڵ��ַ,��ҪΪ��ʹ��ACK	  
  	NRF24L01_Write_Buf(SPI_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH); 

    //ʹ��ͨ��0���Զ�Ӧ��    
  	NRF24L01_Write_Reg(SPI_WRITE_REG+EN_AA,0x01);     
    //ʹ��ͨ��0�Ľ��յ�ַ  
  	NRF24L01_Write_Reg(SPI_WRITE_REG+EN_RXADDR,0x01); 
    //�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
  	NRF24L01_Write_Reg(SPI_WRITE_REG+SETUP_RETR,0x1a);
    //����RFͨ��Ϊ40
  	NRF24L01_Write_Reg(SPI_WRITE_REG+RF_CH,40);       
    //����TX�������,0db����,2Mbps,���������濪��   
  	NRF24L01_Write_Reg(SPI_WRITE_REG+RF_SETUP,0x0f);  //0x27  250K   0x07 1M
    //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,PRIM_RX����ģʽ,���������ж�
  	NRF24L01_Write_Reg(SPI_WRITE_REG+NCONFIG,0x0e);    
    // CEΪ��,10us����������
	Set_NRF24L01_CE;                                  
}		  




