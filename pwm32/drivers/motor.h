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
	float expLocation;   //�������λ�� ����
	float errLocation;   //�Ƕ�λ�����
	
	u8  motorStop;     //���ͣת��־λ 0 ת�� 1ͣת  
	u8  motorDirection;   //1 ˳ʱ����ת  0 ��ת
	float motorSpeed;      //ת��
	
	u8  motorCounter1;    //������
	u16 motorStartUpPwm;  //���������ʼPWM����
	
	float motorLocation;    //�������λ�� �Ƕ� 
	float motorRelLocation; //������λ�� �Ƕ�
	float hallcLocation;   //������ �Ƕ� 0 90 ��180 -90
	float deltaLocation;   //�������жϲ��ٺ� ����ʱ������ĽǶ�λ��
	
	float relativeSpeed;   //���������������ٶ� ��λ  ��/s
	u16 	relativeLocTime;   //�����������жϵ�ʱ��
	u8  	hallcInterruptTimes; //����������жϵĴ��� �������κ��ټ����ٶ�
	u8  	hallcAvaiable;       //����������жϵĴ��� �������κ��ټ����ٶ� hallcAvaiable=1
	u8  	hallAvaiable;    //���������� =1 ʱ������    
	
	float shellLocation;   //�����λ�� �شŽ�
	float shellSpeed;      //����ת��
	
	int16_t  motorHallSteps;  //����Ļ���ת������
	
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
