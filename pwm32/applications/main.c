#include "include.h"
#include "pwm_out.h"
#include "PID.h"
#include "usart.h"
#include "motor.h"

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{ 
  while (1)
  {
		
		
  }
}
#endif


RCC_ClocksTypeDef RCC_Clocks;

void All_Init() 
{
	NVIC_PriorityGroupConfig(NVIC_GROUP);		//中断优先级组别设置
	
	SysTick_Configuration(); 	
	
	HALL_Configuration();
	
	PWM_Out_Init();
	
	TIM_INIT(); //测霍尔环中断时间 测相对转速
	
	PID_set();
	
	Usart1_Init(115200);
	
	RCC_GetClocksFreq(&RCC_Clocks);
	if(RCC_GetSYSCLKSource() == 0x00)
		TIM_SetCompare1(TIM1,0);  
	else if(RCC_GetSYSCLKSource() == 0x04)
		TIM_SetCompare1(TIM1,0);  
	else if(RCC_GetSYSCLKSource() == 0x08)
		TIM_SetCompare1(TIM1,0);  
}

int main(void)
{
   All_Init();
	//Motor_Static();
	Delay_ms(1000);
	
	motor.hallcAvaiable = 0;
	motor.motorRelLocation = 0;
	motor.motorLocation = 0;
	motor.deltaLocation = 0;
	
//	motor.motorStop=1;
	motor.motorDirection=0;
	while(1)
	{
		Duty_Loop(); 
	}	
}










