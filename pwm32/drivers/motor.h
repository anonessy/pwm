#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "stm32f4xx.h"


extern u16 hall,hallc;
extern u16 setPWM; 
extern u16 timerValue;
extern float reltimerValue;

void HALL_Configuration(void);

typedef struct
{
	float expLocation;   //电机期望位置 绝对
	float errLocation;   //角度位置误差
	
	u8  motorStop;     //电机停转标志位 0 转到 1停转  
	u8  motorDirection;   //1 顺时针正转  0 反转
	float motorSpeed;      //转速
	
	u8  motorCounter1;    //计数器
	u16 motorStartUpPwm;  //电机启动初始PWM脉宽
	
	float motorLocation;    //电机绝对位置 角度 
	float motorRelLocation; //电机相对位置 角度
	float hallcLocation;   //霍尔环 角度 0 90 ±180 -90
	float deltaLocation;   //霍尔环中断测速后 依据时间推算的角度位置
	
	float relativeSpeed;   //霍尔环测出的相对速度 单位  °/s
	u16 	relativeLocTime;   //霍尔环两次中断的时间
	u8  	hallcInterruptTimes; //进入霍尔环中断的次数 大于两次后再计算速度
	u8  	hallcAvaiable;       //进入霍尔环中断的次数 大于两次后再计算速度 hallcAvaiable=1
	u8  	hallAvaiable;    //霍尔传感器 =1 时不可用    
	
	float shellLocation;   //弹体的位置 地磁角
	float shellSpeed;      //弹体转速
	
	int16_t  motorHallSteps;  //电机的霍尔转动步数
	
	u8  	kdAvaiable; 
	
}_motor;

extern _motor motor;

void Hall_SW(void);
void HallC_SW(void);
void Motor_Static (void);
void Motor_StartUp (void);
void GetSpeed(void);
void GetErrLocation(void);
void StopModelPWM(void);
void HallErr(void);

# endif
