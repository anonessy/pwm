#ifndef _USART_H
#define _USART_H

#include "stm32f4xx.h"

extern u8 receveok;
extern u8 sendd[15];
extern u8 recevedate[40];

void Usart1_Init(u32 br_num);
void Usart1_IRQ(void);
void Usart1_Send(unsigned char *DataToSend ,u8 data_num);
void SendToFC(void);
	
#endif
