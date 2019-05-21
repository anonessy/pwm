#ifndef __PID_H
#define __PID_H

#include "stm32f4xx.h"

#define safe_div(numerator,denominator,safe_value) ( (denominator == 0)? (safe_value) : ((numerator)/(denominator)) )
#define LIMIT( x,min,max ) ( ((x) <= (min)) ? (min) : ( ((x) > (max))? (max) : (x) ) )
																			
/*=====================================================================================================================
						 *****
=====================================================================================================================*/
typedef struct
{
	int exp;
	int fb;
	int err;
	int out;
	
}_loc_ctrl_st;

typedef struct
{
	u8 fb_d_mode;
	float kp;			 //比例系数
	float ki;			 //积分系数
	float kd;		 //微分系数
	float kd_fb; //previous_d 微分先行
//	float inc_hz;  //不完全微分低通系数
//	float k_inc_d_norm; //Incomplete 不完全微分 归一（0,1）
	float k_ff;		 //前馈 

}_PID_arg_st;

typedef struct
{
	float err;
	float err_old;
	float exp_old;
	float feedback_old;
	
	float fb_d;
	float fb_d_ex;
	float exp_d;
//	float err_d_lpf;
	float err_p;
	float err_i;
	float ff;
	float pre_d;

	float out;    //输出
	
}_PID_val_st;

float PID_calculate( float T,            //周期
							float in_ff,				//前馈
							float expect,				//期望值（设定值）
							float feedback,			//反馈值
							_PID_arg_st *pid_arg, //PID参数结构体
							_PID_val_st *pid_val,	//PID数据结构体
							float inte_d_lim,
							float inte_lim	,		//integration limit，积分限幅
							  int err,
							   u8 switch_err
								);			//输出

extern void PIDspeed(u8 dt);
extern void PIDlocation(u8 dt);
extern void PID_set(void);	
extern float Geog_Filter(float Hy);							  
u16 my_abs(int16_t f);
							
#endif
