/***************************************************************/
#include "stm32f10x.h"
/***************************************************************/
#ifndef __ADC_H
#define __ADC_H 
/***************************************************************/
//PA0~PA7 is 0~7 Channel	
//PB0~PB1 is 8,9 Channel
//PC0~PC5 is 10~15 Channel
/***************************************************************/

void ADC_Configuration(ADC_TypeDef* ADC,u16 ADC_Channel,float SampleTime);
int Get_ADC_Voltage(ADC_TypeDef* ADCx);



#endif


