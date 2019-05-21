#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include "stm32f4xx.h"

typedef struct
{
	u8 check_flag;
	u8 err_flag;
	s16 cnt_1ms;
	s16 cnt_2ms;
	s16 cnt_5ms;
	s16 cnt_10ms;

	u16 time;
}loop_t;

extern loop_t loop;

void Duty_Loop(void);

#endif

