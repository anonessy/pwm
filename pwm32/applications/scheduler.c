#include "scheduler.h"
#include "include.h"
#include "time.h"
#include "PID.h"
#include "usart.h"
#include "motor.h"

void Duty_1ms()
{	
	GetErrLocation();
}

void Duty_2ms()
{	
	SendToFC();
	PIDspeed(2);
}

void Duty_5ms()
{
	PIDlocation(5);
}

u16 t2s = 0;
u16 t500ms = 0;
u16 t500ms2 = 0;
u8 tflag = 1;
u8 tflag2 = 0;
float ee2speed = 0;
float ee1speed = 0;
float ee3speed = 0;

void Duty_10ms()
{	

//	t2s++;
//	if(t2s>=200)
//	{
//		t2s = 200;
//		
//		if(tflag)
//		{
//			motor.shellSpeed = 3600; //+    50--80
//			t500ms++;
//			if(t500ms>=30)
//			{
//				ee2speed = motor.motorSpeed;
//				t500ms = 0;
//				tflag = 0;
//				tflag2 = 1;
//			}
//		}
//		if(tflag2)
//		{
//			motor.shellSpeed = 3600;
//			t500ms2++;
//			if(t500ms2>=30)
//			{
//				ee3speed = motor.motorSpeed;
//				t500ms2 = 0;
//				tflag2 = 0;
//			}
//		}
//	}
//	else
//	{
//		motor.shellSpeed = 3600;
//		
//		ee1speed = motor.motorSpeed; 
//	}

}

void Duty_Loop()   				//最短任务周期为1ms，总的代码执行时间需要小于1ms。
{	
		
	if( loop.check_flag == 1 )
	{
		Duty_1ms();							//周期1ms的任务
		
		if( loop.cnt_2ms >= 2 )
		{
			loop.cnt_2ms = 0;
			Duty_2ms();						//周期2ms的任务
		}
		if( loop.cnt_5ms >= 5 )
		{
			loop.cnt_5ms = 0;
			Duty_5ms();						//周期5ms的任务
		}
		if( loop.cnt_10ms >= 10 )
		{
			loop.cnt_10ms = 0;
			Duty_10ms();					//周期10ms的任务
		}

		loop.check_flag = 0;		//循环运行完毕标志
	}
}






