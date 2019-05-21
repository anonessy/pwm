#include "pwm_out.h"
#include "include.h"

//2分频 168000000 / 2 = 84000000 84M
//频率 35000
//重装载值 2400

void PWM_Out_Init()//400hz
{
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOE, ENABLE);
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
	
	
	TIM_DeInit(TIM1);
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_OCStructInit(&TIM_OCInitStructure);

/////////////////////////////////////////////////////////////////////////////

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_10 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;
	GPIO_Init(GPIOE, &GPIO_InitStructure); 
	
	
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource8, GPIO_AF_TIM1);
	//GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_TIM1); 
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource10, GPIO_AF_TIM1);
	//GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource12, GPIO_AF_TIM1); 
	//GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_TIM1);
	
	/* Compute the prescaler value */
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 2399;				
	TIM_TimeBaseStructure.TIM_Prescaler = 1;		    
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);


	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OCInitStructure.TIM_Pulse = 0;
	
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);

	TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
	TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
	TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;
	TIM_BDTRInitStructure.TIM_DeadTime = 0;
	TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;
	TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;
	TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
	TIM_BDTRConfig(TIM1, &TIM_BDTRInitStructure);

	TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM1,TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM1,TIM_OCPreload_Enable);

	TIM_CtrlPWMOutputs(TIM1, ENABLE);
	TIM_ARRPreloadConfig(TIM1, ENABLE);
	TIM_Cmd(TIM1, ENABLE);	
	
	TIM_CCxCmd(TIM1,TIM_Channel_1,TIM_CCx_Disable);
	TIM_CCxNCmd(TIM1,TIM_Channel_1,TIM_CCxN_Disable);
	TIM_CCxCmd(TIM1,TIM_Channel_2,TIM_CCx_Disable);
	TIM_CCxNCmd(TIM1,TIM_Channel_2,TIM_CCxN_Disable);
	TIM_CCxCmd(TIM1,TIM_Channel_3,TIM_CCx_Disable);
	TIM_CCxNCmd(TIM1,TIM_Channel_3,TIM_CCxN_Disable);
	
	
}


