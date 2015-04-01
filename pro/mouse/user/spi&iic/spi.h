/***************************************************************/
#include "stm32f10x.h"
/***************************************************************/
#ifndef __SPI_H
#define __SPI_H 
/***************************************************************/


//SPI3 B3 B4 B5 ±ªJTAG ’º”√
u8 SPI1_ReadWriteByte(u8 TxData);
u8 SPI2_ReadWriteByte(u8 TxData);
u8 SPI3_ReadWriteByte(u8 TxData);

void SPI1_Configuration(void);
void SPI2_Configuration(void);
void SPI3_Configuration(void);







#endif


