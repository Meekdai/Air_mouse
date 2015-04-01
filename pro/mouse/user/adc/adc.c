/***************************************************************/
#include "stm32f10x.h"
#include "adc.h"
/***************************************************************/
int AD_value;									
unsigned int Precent,Voltage;				
/***************************************************************/

void ADC_Configuration(ADC_TypeDef* ADC,u16 ADC_Channel,float SampleTime)
{
	u8 i=0;
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	if(ADC==ADC1){RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);}
	else if(ADC==ADC2){RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);}
	else if(ADC==ADC3){RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);}
	
	if(ADC_Channel==0){GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;i=1;}
	else if(ADC_Channel==1){GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;i=1;}
	else if(ADC_Channel==2){GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;i=1;}
	else if(ADC_Channel==3){GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;i=1;}
	else if(ADC_Channel==4){GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;i=1;}
	else if(ADC_Channel==5){GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;i=1;}
	else if(ADC_Channel==6){GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;i=1;}
	else if(ADC_Channel==7){GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;i=1;}
	else if(ADC_Channel==8){GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;i=2;}
	else if(ADC_Channel==9){GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;i=2;}
	else if(ADC_Channel==10){GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;i=3;}
	else if(ADC_Channel==11){GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;i=3;}
	else if(ADC_Channel==12){GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;i=3;}
	else if(ADC_Channel==13){GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;i=3;}
	else if(ADC_Channel==14){GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;i=3;}
	else if(ADC_Channel==15){GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;i=3;}
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	       //�˿�ģʽΪģ�����뷽ʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   //��ת�ٶ�Ϊ50M
	if(i==1){GPIO_Init(GPIOA, &GPIO_InitStructure);}
	else if(i==2){GPIO_Init(GPIOB, &GPIO_InitStructure);}
	else if(i==3){GPIO_Init(GPIOC, &GPIO_InitStructure);}
	
	/* ADC ģ����ת������ */
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	             //ADC1��ADC2��������ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;			             //ADC����Ϊ��ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	 			 //����Ϊ����ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//��������ƿ�ʼת��
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	   		 //AD�����ֵΪ�Ҷ˶��뷽ʽ
	ADC_InitStructure.ADC_NbrOfChannel = ADC_Channel;					         //ָ��Ҫ����ADת�����ŵ�
	ADC_Init(ADC, &ADC_InitStructure);						         //������Ĳ�����ʼ��ADC
		/* ADC1�ŵ�1��ת��ͨ������ */
	if     (SampleTime==1.5)  {ADC_RegularChannelConfig(ADC, ADC_Channel, 1, ADC_SampleTime_1Cycles5);}
	else if(SampleTime==7.5)  {ADC_RegularChannelConfig(ADC, ADC_Channel, 1, ADC_SampleTime_7Cycles5);}
	else if(SampleTime==13.5) {ADC_RegularChannelConfig(ADC, ADC_Channel, 1, ADC_SampleTime_13Cycles5);}
	else if(SampleTime==28.5) {ADC_RegularChannelConfig(ADC, ADC_Channel, 1, ADC_SampleTime_28Cycles5);}
	else if(SampleTime==41.5) {ADC_RegularChannelConfig(ADC, ADC_Channel, 1, ADC_SampleTime_41Cycles5);}
	else if(SampleTime==55.5) {ADC_RegularChannelConfig(ADC, ADC_Channel, 1, ADC_SampleTime_55Cycles5);}
	else if(SampleTime==71.5) {ADC_RegularChannelConfig(ADC, ADC_Channel, 1, ADC_SampleTime_71Cycles5);}
	else if(SampleTime==239.5){ADC_RegularChannelConfig(ADC, ADC_Channel, 1, ADC_SampleTime_239Cycles5);}
	
	ADC_Cmd(ADC, ENABLE);                                 			 //ʹ��ADC
	ADC_ResetCalibration(ADC);							 			 //����ADCУ׼�Ĵ���
	while(ADC_GetResetCalibrationStatus(ADC));			 			 //�õ�����У׼�Ĵ���״̬
	ADC_StartCalibration(ADC);							 			 //��ʼУ׼ADC
	while(ADC_GetCalibrationStatus(ADC));				 			 //�õ�У׼�Ĵ���״̬
	ADC_SoftwareStartConvCmd(ADC, ENABLE);				 			 //ʹ��ADC��������ƿ�ʼת��
}

int Get_ADC_Voltage(ADC_TypeDef* ADCx)
{
	AD_value=ADC_GetConversionValue(ADCx);		//��ȡADCת������ֵ
	Precent = (AD_value*100/4096);			    //����ٷֱ�
	Voltage = Precent*33;						//3.3V�ĵ�ƽ�������Ч��ƽ
	return(Voltage);
}


