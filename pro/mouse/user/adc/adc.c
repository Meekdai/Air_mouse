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
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	       //端口模式为模拟输入方式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   //翻转速度为50M
	if(i==1){GPIO_Init(GPIOA, &GPIO_InitStructure);}
	else if(i==2){GPIO_Init(GPIOB, &GPIO_InitStructure);}
	else if(i==3){GPIO_Init(GPIOC, &GPIO_InitStructure);}
	
	/* ADC 模数器转换配置 */
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	             //ADC1和ADC2独立工作模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;			             //ADC设置为单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	 			 //设置为连续转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//由软件控制开始转换
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	   		 //AD输出数值为右端对齐方式
	ADC_InitStructure.ADC_NbrOfChannel = ADC_Channel;					         //指定要进行AD转换的信道
	ADC_Init(ADC, &ADC_InitStructure);						         //用上面的参数初始化ADC
		/* ADC1信道1的转换通道配置 */
	if     (SampleTime==1.5)  {ADC_RegularChannelConfig(ADC, ADC_Channel, 1, ADC_SampleTime_1Cycles5);}
	else if(SampleTime==7.5)  {ADC_RegularChannelConfig(ADC, ADC_Channel, 1, ADC_SampleTime_7Cycles5);}
	else if(SampleTime==13.5) {ADC_RegularChannelConfig(ADC, ADC_Channel, 1, ADC_SampleTime_13Cycles5);}
	else if(SampleTime==28.5) {ADC_RegularChannelConfig(ADC, ADC_Channel, 1, ADC_SampleTime_28Cycles5);}
	else if(SampleTime==41.5) {ADC_RegularChannelConfig(ADC, ADC_Channel, 1, ADC_SampleTime_41Cycles5);}
	else if(SampleTime==55.5) {ADC_RegularChannelConfig(ADC, ADC_Channel, 1, ADC_SampleTime_55Cycles5);}
	else if(SampleTime==71.5) {ADC_RegularChannelConfig(ADC, ADC_Channel, 1, ADC_SampleTime_71Cycles5);}
	else if(SampleTime==239.5){ADC_RegularChannelConfig(ADC, ADC_Channel, 1, ADC_SampleTime_239Cycles5);}
	
	ADC_Cmd(ADC, ENABLE);                                 			 //使能ADC
	ADC_ResetCalibration(ADC);							 			 //重置ADC校准寄存器
	while(ADC_GetResetCalibrationStatus(ADC));			 			 //得到重置校准寄存器状态
	ADC_StartCalibration(ADC);							 			 //开始校准ADC
	while(ADC_GetCalibrationStatus(ADC));				 			 //得到校准寄存器状态
	ADC_SoftwareStartConvCmd(ADC, ENABLE);				 			 //使能ADC由软件控制开始转换
}

int Get_ADC_Voltage(ADC_TypeDef* ADCx)
{
	AD_value=ADC_GetConversionValue(ADCx);		//读取ADC转换出的值
	Precent = (AD_value*100/4096);			    //算出百分比
	Voltage = Precent*33;						//3.3V的电平，计算等效电平
	return(Voltage);
}


