#include "PID.h"
#include "include.h"
#include "motor.h"
#include "usart.h"

_loc_ctrl_st sped_ctrl;
_loc_ctrl_st loca_ctrl;

//速度环控制参数
_PID_arg_st sped_arg; 
_PID_arg_st loca_arg;

//速度环控制数据
_PID_val_st sped_val; 
_PID_val_st loca_val; 
 
//速度获取频率最快40hz 速度环500hz 微分平时无作用？
void PID_set()	
{		                        	
	sped_arg.kp = 0.6f;     // 0.7
	sped_arg.ki = 0.5f;     //  		
	sped_arg.kd = 0.2f;		// 0.2	 	   
	sped_arg.kd_fb = 0.00;
	sped_arg.k_ff = 0.00f;
	
	loca_arg.kp = 30.0f;  
}

void PIDlocation(u8 dt)
{
	if(motor.motorStop) //电机停止时 清0
	{	
		loca_val.err = 0;
		loca_val.err_i=0;
		loca_val.out = 0;
	}
	else
	{
//			if((motor.errLocation > 10)&&(motor.errLocation < 350)) 
//			{
				loca_ctrl.err = motor.errLocation;      //范围：1-180 典型值：5-80
				PID_calculate( dt*1e-3f,            //周期（单位：秒）
									0 ,						
									loca_ctrl.exp ,		//期望值（设定值）
									loca_ctrl.fb ,	      //反馈值（）
									&loca_arg,           //PID参数结构体
									&loca_val,	         //PID数据结构体
									0,                      //积分误差限幅
									0,  			          //integration limit，积分限幅
									loca_ctrl.err,
									1
									);	
				
				if(loca_val.out>=3600) //3600°/s  //范围：30-5400 典型值：150-2400
					loca_val.out = 3600;
				
//				motor.motorCounter1 = 0;
//			}
//			else
//			{
//				motor.motorStop = 1; 
//				
//				reltimerValue = 0;	
//				motor.hallcInterruptTimes = 0;
//				motor.relativeSpeed = 0;
//				motor.hallcAvaiable = 0;
//				
//				setPWM = 400;
//				if(TIM1->CCR1!=0)
//					TIM1->CCR1 = setPWM;		
//				else if (TIM1->CCR2 != 0)
//					TIM1->CCR2 = setPWM;
//				else if (TIM1->CCR3 != 0)
//					TIM1->CCR3 = setPWM;						
////				if(motor.motorCounter1 == 0)
////				{
////					motor.motorCounter1 = 1;
////					loca_val.out = motor.motorSpeed; //到达期望位置后 就是定速
////				}
//			}
		}
	
}

u8 feedpwm = 0;
float oldshellSpeed = 0;
void PIDspeed(u8 dt)
{
	if(motor.motorStop) //电机为 关闭 状态 不进行计算
	{
		sped_val.err = 0;
		sped_val.err_i=0;
		sped_val.out = 0;
		
		//setPWM = 300;
		//StopModelPWM();
	}
 	else
	{
//	   	if(motor.shellSpeed >= 720)  //弹体速度大于 -- 速度控制
//			{
				//LIMIT(loca_val.out,-400,400);                       //pwm100 - 600 °/s
				
//				if(motor.motorSpeed > (motor.shellSpeed*0.9f))			
//					sped_ctrl.exp = motor.shellSpeed + loca_val.out;    //shellSpeed 范围：1080-3600 典型值：1800-2800 loca_val.out -200 - 200
//				else
//					sped_ctrl.exp = motor.shellSpeed;
				
				sped_ctrl.exp = motor.shellSpeed;
				sped_ctrl.fb  = motor.motorSpeed;   //电机的绝对速度 单位 °/s
				PID_calculate( dt*1e-3f,              //周期（单位：秒）
									0 ,						
									sped_ctrl.exp ,		//期望值（设定值）
									sped_ctrl.fb ,	      //反馈值（）
									&sped_arg,           //PID参数结构体
									&sped_val,	         //PID数据结构体
									300,                      //积分误差限幅
									400,   		      	  //integration limit，积分限幅
									0,
									0
									 );

			//if((motor.shellSpeed-oldshellSpeed)>200)
			//{
					feedpwm = 110 + (motor.shellSpeed-2100)/20.0f;
					if(feedpwm<=50)
						feedpwm = 50;
					if(feedpwm>=185)
						feedpwm =185;
			//	}
				setPWM = feedpwm + sped_val.out;      //sped_val.out 范围：54-180 90-140
				
				if(setPWM<=35)
					setPWM = 35;
				if(setPWM>=400)
					setPWM =400;
				
				//oldshellSpeed = motor.shellSpeed;
//			} 								
//			else  	//无速度控制
//			{
//				setPWM = 200 + loca_val.out * 0.7f;
//				if(setPWM<=50)
//					setPWM = 50;
//				if(setPWM>=600)
//					setPWM =600;
//			}
	}
}

float PID_calculate( float dT_s,             //周期（单位：秒）
							float in_ff,			
							float expect,				//期望值（设定值）
							float feedback,			//反馈值（）
							_PID_arg_st *pid_arg,   //PID参数结构体
							_PID_val_st *pid_val,	//PID数据结构体
							float inte_d_lim,       //积分误差限幅
							float inte_lim	,		   //integration limit，积分限幅	
							int	err,
							u8  switch_err	
							)	
{
//	float hz; 
//	hz = safe_div(1.0f,dT_s,0);
	
//	if(switch_err)
//		pid_val->err = err; 
//	else
		pid_val->err = expect - feedback;
	
	LIMIT(pid_val->err,-100,100);
	
	pid_val->err_p =  pid_arg->kp * pid_val->err;
	
	if(motor.kdAvaiable)
	{
		motor.kdAvaiable = 0;
		pid_val->exp_d = pid_arg->kd * (pid_val->err - pid_val->err_old); //* hz;   //本次期望 - 上次期望
	}
		
	///积分及限幅///
	pid_val->err_i += pid_arg->ki * LIMIT((pid_val->err ),-inte_d_lim,inte_d_lim ) * dT_s;//)*T;//+ differential/pid_arg->kp 
	
	pid_val->err_i = LIMIT(pid_val->err_i,-inte_lim,inte_lim);
	
	pid_val->out = pid_val->err_p
					 + pid_val->exp_d
					 + pid_val->err_i;

	pid_val->err_old = pid_val->err;
	
	return (pid_val->out);
}

u16 my_abs(int16_t f)
{
	if (f >= 0)
	{
		return f;
	}
	return -f;
}

//---------------------------------------------------------------------------
// oˉêy??3?: Geog_Filter                                                    -
// oˉêy?èê?: μ?′?êy?Y??????2¨                                               -
// oˉêyè??ú:                                                                -
// oˉêy3??ú: ?T                                                             -
// ′′?¨è??ú: 2018/11/28                                                     -
// DT??è??ú: LZC 2018.11.24                                                 -
// DT???-òò:                                                                -
//---------------------------------------------------------------------------
//float Geog_Filter(float Hy)
//{
//	static float Hya_d1 = 0.0;
//	static float Hya_a1 = 0.0;

//	static float Hya_a = 0.0,Hya = 0.0;	
//	static char flag_c = 0;
//	
//	if (flag_c == 0)
//	{
//		Hya_d1 = 0;
//		Hya_a1 = 0;
//		Hya_a = 0.2 * Hy + 0.2 * Hya_d1 + 0.6 * Hya_a1;
//		Hya_d1 = Hya;
//		Hya_a1 = Hya_a;
//				
//	}
//	else
//	{
//		Hya_a = 0.2 * Hy + 0.2 * Hya_d1 + 0.6 * Hya_a1;
//		Hya_d1 = Hya;
//		Hya_a1 = Hya_a;								
//	}
//	return Hya_a;	
//}























