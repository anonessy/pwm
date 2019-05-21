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
	float kp;			 //����ϵ��
	float ki;			 //����ϵ��
	float kd;		 //΢��ϵ��
	float kd_fb; //previous_d ΢������
//	float inc_hz;  //����ȫ΢�ֵ�ͨϵ��
//	float k_inc_d_norm; //Incomplete ����ȫ΢�� ��һ��0,1��
	float k_ff;		 //ǰ�� 

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

	float out;    //���
	
}_PID_val_st;

float PID_calculate( float T,            //����
							float in_ff,				//ǰ��
							float expect,				//����ֵ���趨ֵ��
							float feedback,			//����ֵ
							_PID_arg_st *pid_arg, //PID�����ṹ��
							_PID_val_st *pid_val,	//PID���ݽṹ��
							float inte_d_lim,
							float inte_lim	,		//integration limit�������޷�
							  int err,
							   u8 switch_err
								);			//���

extern void PIDspeed(u8 dt);
extern void PIDlocation(u8 dt);
extern void PID_set(void);	
extern float Geog_Filter(float Hy);							  
u16 my_abs(int16_t f);
							
#endif
