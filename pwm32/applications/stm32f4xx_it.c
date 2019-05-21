#include "include.h"
#include "pwm_out.h"
#include "usart.h"
#include "motor.h"

void NMI_Handler(void)
{
}

void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

void SVC_Handler(void)
{
}

void DebugMon_Handler(void)
{
}

void SysTick_Handler(void)
{
	sysTickUptime++;
	sys_time();
}

void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET) //溢出中断//
	{
		reltimerValue = 0;
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //清除中断标志位
}

void TIM5_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM5,TIM_IT_Update)==SET) //溢出中断//
	{

	}
	TIM_ClearITPendingBit(TIM5,TIM_IT_Update);  //清除中断标志位
}

void TIM3_IRQHandler(void)
{

}

void TIM4_IRQHandler(void)
{

}

u16 hhhhall = 0;
//霍尔中断
void EXTI0_IRQHandler(void) 
{
	if(EXTI_GetITStatus(EXTI_Line0)!= RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line0);
		
		hall = GPIO_ReadInputData(GPIOA);
		hall = hall & 0x007; 
		if(motor.motorDirection )hall=7-hall;
		Hall_SW();
	}	
}
void EXTI1_IRQHandler(void) 
{
	if(EXTI_GetITStatus(EXTI_Line1)!= RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line1);
		
		hall = GPIO_ReadInputData(GPIOA);
		hall = hall & 0x007; 
		if(motor.motorDirection )hall=7-hall;
	   Hall_SW();
	}	
}
void EXTI2_IRQHandler(void) 
{
	if(EXTI_GetITStatus(EXTI_Line2)!= RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line2);
		
		hall = GPIO_ReadInputData(GPIOA);
		hall = hall & 0x007; 
		if(motor.motorDirection )hall=7-hall;
		Hall_SW();
	}	
}

//霍尔环中断
void EXTI3_IRQHandler(void) //13 180
{
	if(EXTI_GetITStatus(EXTI_Line3)!= RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line3);
		motor.hallcLocation = 180;
		HallC_SW();
		
	}	
}
void EXTI4_IRQHandler(void) //11 90
{
	if(EXTI_GetITStatus(EXTI_Line4)!= RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line4);
		
		motor.hallcLocation = 90;
		HallC_SW();
	
	}	
}
void EXTI9_5_IRQHandler(void)   
{
	if(EXTI_GetITStatus(EXTI_Line5)!= RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line5);//7 0
		
		if(motor.motorDirection)
		{
			motor.hallcLocation = 0;
		}
		else
		{
			motor.hallcLocation = 360;
		}
		
		HallC_SW();
	}
	
	if(EXTI_GetITStatus(EXTI_Line6)!= RESET)//14 270
	{
		EXTI_ClearITPendingBit(EXTI_Line6);
		
		motor.hallcLocation = 270;
		HallC_SW();
	}
}
      

//void EXTI3_IRQHandler(void) //13  90
//{
//	if(EXTI_GetITStatus(EXTI_Line3)!= RESET)
//	{
//		EXTI_ClearITPendingBit(EXTI_Line3);
//		
//		if(motor.motorDirection)
//		{
//			motor.hallcLocation = 0;
//		}
//		else
//		{
//			motor.hallcLocation = 360;
//		}
//		HallC_SW();
//	}	
//}
//void EXTI4_IRQHandler(void) //13  90
//{
//	if(EXTI_GetITStatus(EXTI_Line4)!= RESET)
//	{
//		EXTI_ClearITPendingBit(EXTI_Line4);
//		
//		motor.hallcLocation = 270;
//		HallC_SW();
//	}	
//}
//void EXTI9_5_IRQHandler(void)   
//{
//	if(EXTI_GetITStatus(EXTI_Line5)!= RESET)
//	{
//		EXTI_ClearITPendingBit(EXTI_Line5);//11 0
//		
//		motor.hallcLocation = 180;
//		HallC_SW();
//	}
//	
//	if(EXTI_GetITStatus(EXTI_Line6)!= RESET)
//	{
//		EXTI_ClearITPendingBit(EXTI_Line6);//霍尔环
//		
//		motor.hallcLocation = 90;
//		HallC_SW();
//	}
//}

void USART1_IRQHandler (void)
{
	
}

void USART2_IRQHandler(void)
{

}

void USART3_IRQHandler (void)
{
	Usart1_IRQ();
}



