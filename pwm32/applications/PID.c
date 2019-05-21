#include "PID.h"
#include "include.h"
#include "motor.h"
#include "usart.h"

_loc_ctrl_st sped_ctrl;
_loc_ctrl_st loca_ctrl;

//�ٶȻ����Ʋ���
_PID_arg_st sped_arg; 
_PID_arg_st loca_arg;

//�ٶȻ���������
_PID_val_st sped_val; 
_PID_val_st loca_val; 
 
//�ٶȻ�ȡƵ�����40hz �ٶȻ�500hz ΢��ƽʱ�����ã�
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
	if(motor.motorStop) //���ֹͣʱ ��0
	{	
		loca_val.err = 0;
		loca_val.err_i=0;
		loca_val.out = 0;
	}
	else
	{
//			if((motor.errLocation > 10)&&(motor.errLocation < 350)) 
//			{
				loca_ctrl.err = motor.errLocation;      //��Χ��1-180 ����ֵ��5-80
				PID_calculate( dt*1e-3f,            //���ڣ���λ���룩
									0 ,						
									loca_ctrl.exp ,		//����ֵ���趨ֵ��
									loca_ctrl.fb ,	      //����ֵ����
									&loca_arg,           //PID�����ṹ��
									&loca_val,	         //PID���ݽṹ��
									0,                      //��������޷�
									0,  			          //integration limit�������޷�
									loca_ctrl.err,
									1
									);	
				
				if(loca_val.out>=3600) //3600��/s  //��Χ��30-5400 ����ֵ��150-2400
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
////					loca_val.out = motor.motorSpeed; //��������λ�ú� ���Ƕ���
////				}
//			}
		}
	
}

u8 feedpwm = 0;
float oldshellSpeed = 0;
void PIDspeed(u8 dt)
{
	if(motor.motorStop) //���Ϊ �ر� ״̬ �����м���
	{
		sped_val.err = 0;
		sped_val.err_i=0;
		sped_val.out = 0;
		
		//setPWM = 300;
		//StopModelPWM();
	}
 	else
	{
//	   	if(motor.shellSpeed >= 720)  //�����ٶȴ��� -- �ٶȿ���
//			{
				//LIMIT(loca_val.out,-400,400);                       //pwm100 - 600 ��/s
				
//				if(motor.motorSpeed > (motor.shellSpeed*0.9f))			
//					sped_ctrl.exp = motor.shellSpeed + loca_val.out;    //shellSpeed ��Χ��1080-3600 ����ֵ��1800-2800 loca_val.out -200 - 200
//				else
//					sped_ctrl.exp = motor.shellSpeed;
				
				sped_ctrl.exp = motor.shellSpeed;
				sped_ctrl.fb  = motor.motorSpeed;   //����ľ����ٶ� ��λ ��/s
				PID_calculate( dt*1e-3f,              //���ڣ���λ���룩
									0 ,						
									sped_ctrl.exp ,		//����ֵ���趨ֵ��
									sped_ctrl.fb ,	      //����ֵ����
									&sped_arg,           //PID�����ṹ��
									&sped_val,	         //PID���ݽṹ��
									300,                      //��������޷�
									400,   		      	  //integration limit�������޷�
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
				setPWM = feedpwm + sped_val.out;      //sped_val.out ��Χ��54-180 90-140
				
				if(setPWM<=35)
					setPWM = 35;
				if(setPWM>=400)
					setPWM =400;
				
				//oldshellSpeed = motor.shellSpeed;
//			} 								
//			else  	//���ٶȿ���
//			{
//				setPWM = 200 + loca_val.out * 0.7f;
//				if(setPWM<=50)
//					setPWM = 50;
//				if(setPWM>=600)
//					setPWM =600;
//			}
	}
}

float PID_calculate( float dT_s,             //���ڣ���λ���룩
							float in_ff,			
							float expect,				//����ֵ���趨ֵ��
							float feedback,			//����ֵ����
							_PID_arg_st *pid_arg,   //PID�����ṹ��
							_PID_val_st *pid_val,	//PID���ݽṹ��
							float inte_d_lim,       //��������޷�
							float inte_lim	,		   //integration limit�������޷�	
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
		pid_val->exp_d = pid_arg->kd * (pid_val->err - pid_val->err_old); //* hz;   //�������� - �ϴ�����
	}
		
	///���ּ��޷�///
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
// o����y??3?: Geog_Filter                                                    -
// o����y?����?: ��?��?��y?Y??????2��                                               -
// o����y��??��:                                                                -
// o����y3??��: ?T                                                             -
// ���?����??��: 2018/11/28                                                     -
// DT??��??��: LZC 2018.11.24                                                 -
// DT???-����:                                                                -
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























