#include "time.h"
#include "include.h"
#include "scheduler.h"

#define SYS_TIMx					TIM2
#define SYS_RCC_TIMx			RCC_APB1Periph_TIM2

s16 loop_cnt;
loop_t loop;

void TIM_INIT()
{
   TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 
	/* 使能时钟 */
	RCC_APB1PeriphClockCmd(SYS_RCC_TIMx,ENABLE);
	TIM_DeInit(SYS_TIMx);

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Period = 49999;  //重装值49999  5s溢出中断
	TIM_TimeBaseStructure.TIM_Prescaler= 8399; // 10khz 
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;   //向上计数
	TIM_TimeBaseInit(SYS_TIMx,&TIM_TimeBaseStructure);
	
	TIM_ClearFlag(SYS_TIMx,TIM_FLAG_Update);
	TIM_ITConfig(SYS_TIMx,TIM_IT_Update,ENABLE);
	TIM_Cmd(SYS_TIMx, ENABLE);	

	//定时器TIM5 估算位置
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
	TIM_DeInit(TIM5);

	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;	  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Period = 99999;  //重装值10000  10s溢出中断
	TIM_TimeBaseStructure.TIM_Prescaler= 8399; // 10khz 
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;   //向上计数
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);
	
	TIM_ClearFlag(TIM5,TIM_FLAG_Update);
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM5, ENABLE);																		
}

volatile uint32_t sysTickUptime = 0;

#define TICK_PER_SECOND 1000 
#define TICK_US	(1000000/TICK_PER_SECOND)

void  SysTick_Configuration(void)
{
	RCC_ClocksTypeDef  rcc_clocks;
	uint32_t         cnts;

	RCC_GetClocksFreq(&rcc_clocks);

	cnts = (uint32_t)rcc_clocks.HCLK_Frequency / TICK_PER_SECOND;
	cnts = cnts / 8;

	SysTick_Config(cnts);
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	NVIC_SetPriority(SysTick_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),2,0));
}

uint32_t GetSysTime_us(void) 
{
	register uint32_t ms;
	u32 value;
	ms = sysTickUptime;
	value = ms * TICK_US + (SysTick->LOAD - SysTick->VAL) * TICK_US / SysTick->LOAD;
	return value;
}

void Delay_us(uint32_t us)
{
    uint32_t now = GetSysTime_us();
    while (GetSysTime_us() - now < us);
}

void Delay_ms(uint32_t ms)
{
    while (ms--)
        Delay_us(1000);
}

void sys_time()
{ 
	loop.time++; //u16
	loop.cnt_2ms++;
	loop.cnt_5ms++;
	loop.cnt_10ms++;


	if( loop.check_flag == 1)
	{
		loop.err_flag ++;     //每累加一次，证明代码在预定周期内没有跑完。
	}
	else
	{	
		loop.check_flag = 1;	//该标志位在循环的最后被清零
	}	
}

enum
{
	NOW = 0,
	OLD,
	NEW,
};






