#ifndef _TIME_H_
#define _TIME_H_

#include "stm32f4xx.h"

void TIM_INIT(void);
void sys_time(void);

extern volatile uint32_t sysTickUptime;

void Delay_us(uint32_t);
void Delay_ms(uint32_t);
void SysTick_Configuration(void);
uint32_t GetSysTime_us(void);
#endif
