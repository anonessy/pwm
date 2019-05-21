#include "motor.h"
#include "stm32f4xx_gpio.h"
#include "include.h"
#include "usart.h"
#include "PID.h"
//U V W �� �� ��
//A B C �� �� ��
_motor motor={0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

u16 hall = 0;								//������������ֵ
u16 hallc = 0;                      //��������������ֵ
u16 setPWM = 50; 						   //����PWM������            
float reltimerValue = 0;
float timers = 0;

float timerBuf[6] = {0};
u8  timerCounter = 0;
u8  timerStart = 0;
float timerTotal = 0;
float timerResult = 0;
	
//����ĳһ��ʱ ��ʵ������ֹͣ����һ��������һ���Ľ��紦��

u16 hall1=0,hall2=0;
void HallC_SW()
{
	static float motorSpeed_last=0.0f;
	hall1 = GPIO_ReadInputData(GPIOE);  // 0011 1000
	hall1 = hall1>>2;
	hall1 = hall1&0x000E;
	hall2 = GPIO_ReadInputData(GPIOD);  // 0100 0000
	hall2 = hall2>>6;
	hall2 = hall2&0x0001;
	hallc = hall1 + hall2;

	motor.motorHallSteps = 0;
	motor.deltaLocation = 0;

	if(motor.hallcInterruptTimes)
	{
		reltimerValue = TIM_GetCounter(TIM2);
		
		if(reltimerValue)
		{
			motor.motorSpeed = 900000.0f / reltimerValue; 
		
			motor.motorSpeed = 0.9f*motor.motorSpeed + 0.1f*motorSpeed_last;
		
			motorSpeed_last = motor.motorSpeed;
		}
		
		motor.hallcAvaiable = 1; //���λ������жϺ� ����
		motor.kdAvaiable = 1;
	}
	else
	{
		motor.hallcInterruptTimes = 1;
	}
	TIM_SetCounter(TIM2,0);	
}

void Hall_SW(void) //���ϵ���Ϊ��ʱ��
{	
	if(motor.motorStop) 
	{
		
	}
	else
	{
		if((hall>0)&&(hall<7))
		{
			if(motor.motorDirection) //��ת
				motor.motorHallSteps++;
			else     
				motor.motorHallSteps--;
			
			motor.deltaLocation = (float)motor.motorHallSteps * 4.28f;
			if(motor.deltaLocation >= 89)
					motor.deltaLocation = 89;
			if(motor.deltaLocation <= -89)
				motor.deltaLocation = -89;
			
			motor.motorRelLocation = motor.hallcLocation + motor.deltaLocation;
			if(motor.motorRelLocation<0)
				motor.motorRelLocation += 360;
			if(motor.motorRelLocation>360)
				motor.motorRelLocation -= 360;
		}
		
		switch(hall)
		{
			case 5:     //AB
				motor.hallAvaiable = 0;
				GPIO_ResetBits(GPIOE, GPIO_Pin_13);
				TIM_SetCompare3(TIM1,0);  
				TIM_CCxCmd(TIM1,TIM_Channel_3,TIM_CCx_Enable);	
				TIM_CCxNCmd(TIM1,TIM_Channel_3,TIM_CCxN_Enable);
			
				GPIO_ResetBits(GPIOE, GPIO_Pin_9);
				TIM_SetCompare1(TIM1,setPWM);  
				TIM_CCxCmd(TIM1,TIM_Channel_1,TIM_CCx_Enable);	
				TIM_CCxNCmd(TIM1,TIM_Channel_1,TIM_CCxN_Enable);
				
				GPIO_SetBits(GPIOE, GPIO_Pin_11);
				TIM_SetCompare2(TIM1,0);  
				TIM_CCxCmd(TIM1,TIM_Channel_2,TIM_CCx_Enable);	
				TIM_CCxNCmd(TIM1,TIM_Channel_2,TIM_CCxN_Enable);	
				break;	
			case 1:  //AC
				motor.hallAvaiable = 0;
				GPIO_ResetBits(GPIOE, GPIO_Pin_11);
				TIM_SetCompare2(TIM1,0);  
				TIM_CCxCmd(TIM1,TIM_Channel_2,TIM_CCx_Enable);	
				TIM_CCxNCmd(TIM1,TIM_Channel_2,TIM_CCxN_Enable);
			
				GPIO_ResetBits(GPIOE, GPIO_Pin_9);
				TIM_SetCompare1(TIM1,setPWM);  
				TIM_CCxCmd(TIM1,TIM_Channel_1,TIM_CCx_Enable);	
				TIM_CCxNCmd(TIM1,TIM_Channel_1,TIM_CCxN_Enable);
		
				GPIO_SetBits(GPIOE, GPIO_Pin_13);
				TIM_SetCompare3(TIM1,0);  
				TIM_CCxCmd(TIM1,TIM_Channel_3,TIM_CCx_Enable);	
				TIM_CCxNCmd(TIM1,TIM_Channel_3,TIM_CCxN_Enable);		
				break;
			case 3:   //BC
				motor.hallAvaiable = 0;
				GPIO_ResetBits(GPIOE, GPIO_Pin_9);
				TIM_SetCompare1(TIM1,0);  
				TIM_CCxCmd(TIM1,TIM_Channel_1,TIM_CCx_Enable);	
				TIM_CCxNCmd(TIM1,TIM_Channel_1,TIM_CCxN_Enable);
				
				GPIO_ResetBits(GPIOE, GPIO_Pin_11);
				TIM_SetCompare2(TIM1,setPWM);  
				TIM_CCxCmd(TIM1,TIM_Channel_2,TIM_CCx_Enable);	
				TIM_CCxNCmd(TIM1,TIM_Channel_2,TIM_CCxN_Enable);
				
				GPIO_SetBits(GPIOE, GPIO_Pin_13);
				TIM_SetCompare3(TIM1,0);  
				TIM_CCxCmd(TIM1,TIM_Channel_3,TIM_CCx_Enable);	
				TIM_CCxNCmd(TIM1,TIM_Channel_3,TIM_CCxN_Enable);	
				break;
			case 2:   //BA
				motor.hallAvaiable = 0;
				GPIO_ResetBits(GPIOE, GPIO_Pin_13);
				TIM_SetCompare3(TIM1,0);  
				TIM_CCxCmd(TIM1,TIM_Channel_3,TIM_CCx_Enable);	
				TIM_CCxNCmd(TIM1,TIM_Channel_3,TIM_CCxN_Enable);		
			
				GPIO_SetBits(GPIOE, GPIO_Pin_9);
				TIM_SetCompare1(TIM1,0);  
				TIM_CCxCmd(TIM1,TIM_Channel_1,TIM_CCx_Enable);	
				TIM_CCxNCmd(TIM1,TIM_Channel_1,TIM_CCxN_Enable);
				
				GPIO_ResetBits(GPIOE, GPIO_Pin_11);
				TIM_SetCompare2(TIM1,setPWM);  
				TIM_CCxCmd(TIM1,TIM_Channel_2,TIM_CCx_Enable);	
				TIM_CCxNCmd(TIM1,TIM_Channel_2,TIM_CCxN_Enable);
				break;
			case 6:   //CA
				motor.hallAvaiable = 0;
				GPIO_ResetBits(GPIOE, GPIO_Pin_11);
				TIM_SetCompare2(TIM1,0);  
				TIM_CCxCmd(TIM1,TIM_Channel_2,TIM_CCx_Enable);	
				TIM_CCxNCmd(TIM1,TIM_Channel_2,TIM_CCxN_Enable);
			
				GPIO_SetBits(GPIOE, GPIO_Pin_9);
				TIM_SetCompare1(TIM1,0);  
				TIM_CCxCmd(TIM1,TIM_Channel_1,TIM_CCx_Enable);	
				TIM_CCxNCmd(TIM1,TIM_Channel_1,TIM_CCxN_Enable);
						
				GPIO_ResetBits(GPIOE, GPIO_Pin_13);
				TIM_SetCompare3(TIM1,setPWM);  
				TIM_CCxCmd(TIM1,TIM_Channel_3,TIM_CCx_Enable);	
				TIM_CCxNCmd(TIM1,TIM_Channel_3,TIM_CCxN_Enable);	
				break;
			case 4:		 //CB		
				motor.hallAvaiable = 0;
				GPIO_ResetBits(GPIOE, GPIO_Pin_9);
				TIM_SetCompare1(TIM1,0);  
				TIM_CCxCmd(TIM1,TIM_Channel_1,TIM_CCx_Enable);	
				TIM_CCxNCmd(TIM1,TIM_Channel_1,TIM_CCxN_Enable);
				
				GPIO_SetBits(GPIOE, GPIO_Pin_11);
				TIM_SetCompare2(TIM1,0);  
				TIM_CCxCmd(TIM1,TIM_Channel_2,TIM_CCx_Enable);	
				TIM_CCxNCmd(TIM1,TIM_Channel_2,TIM_CCxN_Enable);
				
				GPIO_ResetBits(GPIOE, GPIO_Pin_13);
				TIM_SetCompare3(TIM1,setPWM);  
				TIM_CCxCmd(TIM1,TIM_Channel_3,TIM_CCx_Enable);	
				TIM_CCxNCmd(TIM1,TIM_Channel_3,TIM_CCxN_Enable);	
				break;
			
			default:
//				motor.hallAvaiable = 1; //����������������
//				motor.motorStop = 1;
//				TIM1->CCR1=0;
//				TIM1->CCR2=0; 		  
//				TIM1->CCR3=0;
//				GPIO_ResetBits(GPIOE, GPIO_Pin_8 | GPIO_Pin_10 | GPIO_Pin_12); 
			break;
		}
	}
}	

float speedtimer = 0;
void GetErrLocation() //��������ҪС��180��
{
	speedtimer = TIM_GetCounter(TIM5);
	motor.motorLocation = motor.motorRelLocation + motor.shellLocation; //���û�յ� 	
	//motor.motorLocation = motor.motorLocation + motor.shellSpeed * speedtimer /10000.0f;   //�ٶȴ���С�ڶ���Ҫ����
	if(motor.motorLocation>360)
		motor.motorLocation = motor.motorLocation - 360;
	
	if(motor.shellSpeed >= 720)//�˲� //����ת�ٴ��� -- ��/s�� �����ת ���ٸı���ת�� ���ı�������������-��С��ת���������
	{
		if(motor.expLocation>=motor.motorLocation) //����ֵ ���� ��ǰֵ
		{
			motor.errLocation = motor.expLocation - motor.motorLocation;
			if(motor.errLocation>=180)
			{
				motor.errLocation =  360 + motor.motorLocation - motor.expLocation;
				motor.motorDirection=0; //��ת 
			}
			else{			
				//motor.motorDirection=1; //��ת
				
				motor.motorDirection=0; //��ת 
				motor.errLocation = -motor.errLocation; //PWM��С
			}
		}
		else  //����ֵ С�� ��ǰֵ
		{
			motor.errLocation = motor.motorLocation - motor.expLocation;
			if(motor.errLocation>=180)
			{
				motor.errLocation = 360 - motor.motorLocation + motor.expLocation;			
				//motor.motorDirection=1; //��ת
				
				motor.motorDirection=0; //��ת 
				motor.errLocation = -motor.errLocation; //PWM��С
			}
			else{			
				motor.motorDirection=0; //��ת
			}	
		}
	}
	else
	{
		if(motor.expLocation>=motor.motorLocation) //����ֵ ���� ��ǰֵ
		{
			motor.errLocation = motor.expLocation - motor.motorLocation;
			if(motor.errLocation>=180)
			{
				motor.errLocation =  360 + motor.motorLocation - motor.expLocation;
				motor.motorDirection=0; //��ת
			}
			else{			
				motor.motorDirection=1; //��ת
			}
		}
		else  //����ֵ С�� ��ǰֵ
		{
			motor.errLocation = motor.motorLocation - motor.expLocation;
			if(motor.errLocation>=180)
			{
				motor.errLocation = 360 - motor.motorLocation + motor.expLocation;			
				motor.motorDirection=1; //��ת
			}
			else{			
				motor.motorDirection=0; //��ת
			}	
		}
	}

	if(motor.shellSpeed > 1)
	{
		Motor_StartUp();
	}
//	if((motor.errLocation>=5)&&(motor.errLocation<=355)) //�˴�errֵӦ��PID�е�errֵ��
//	{
//		Motor_StartUp();
//	}	
}

void GetSpeed ()
{
	if(motor.motorStop) //���Ϊ �ر� ״̬ �����м���
	{
//		timerCounter = 0;
//		timerTotal  = 0;
//		timerResult = 0;
//		timerBuf[0] = 0;
//		timerBuf[1] = 0;
//		timerBuf[2] = 0;
//		timerBuf[3] = 0;
//		timerBuf[4] = 0;
//		timerBuf[5] = 0;
//		timerStart = 0;
		motor.motorSpeed = 0;
		
		motor.hallcAvaiable  = 0;
		reltimerValue = 0;
		motor.relativeSpeed = 0;
		
	}else
	{
//		timerTotal -= timerBuf[timerCounter];
//		timerBuf[timerCounter] = reltimerValue;
//		timerTotal += reltimerValue;
//		timerResult = timerTotal/6.0f;
//		timerCounter++;
//		if(timerCounter>=6)
//		{
//			timerStart = 1;
//			timerCounter=0;
//		}

//		if(motor.hallcAvaiable)
//		{
//			//if(timerStart)
//				motor.relativeSpeed = 10000.0f / timerResult * 90.0f; 
//			//else
			//	motor.motorSpeed = 900000.0f / reltimerValue; 
			//motor.motorSpeed = motor.relativeSpeed;
//		}
		
	}
}

void Motor_StartUp()//��������ҪС��180��
{	
	if(motor.motorStop) //������ֹͣ״̬����������ת״̬����������
	{			
//		//�ó���� �� ת��
//		if(motor.expLocation>=motor.motorLocation) //����ֵ ���� ��ǰֵ
//		{
//			motor.errLocation = motor.expLocation - motor.motorLocation;
//			if(motor.errLocation>=180)
//			{
//				motor.errLocation =  360 + motor.motorLocation - motor.expLocation;
//				motor.motorDirection=0; //��ת
//			}
//			else{			
//				motor.motorDirection=1; //��ת
//			}
//		}
//		else  //����ֵ С�� ��ǰֵ
//		{
//			motor.errLocation = motor.motorLocation - motor.expLocation;
//			if(motor.errLocation>=180)
//			{
//				motor.errLocation = 360 - motor.motorLocation + motor.expLocation;			
//				motor.motorDirection=1; //��ת
//			}
//			else{			
//				motor.motorDirection=0; //��ת
//			}	
//		}

		motor.motorStop = 0;  //������� �� 
		setPWM =100; 
		hall=GPIO_ReadInputData(GPIOA);
		hall=hall&0x0007; //0000 0001 1100 0000  // 0111 0000 0000 0000
		if(motor.motorDirection)hall=7-hall;
		Hall_SW();
	}
}

//����ϵ��ת����ȷ��λ�� ,�Ƿ���Ҫǿ�籣��?
void Motor_Static()
{	
	if(motor.motorStop) //������ֹͣ״̬����������ת״̬����������
	{		
		motor.motorStop = 0;  //������� �� 
		setPWM = 300; 
		hall=GPIO_ReadInputData(GPIOA);
		hall=hall&0x0007; //0000 0001 1100 0000  // 0111 0000 0000 0000
		//if(motor.motorDirection)hall=7-hall;
		Hall_SW();
	}
	while(1)
	{
//		if(motor.motorStop) //������ֹͣ״̬����������ת״̬����������
//		{		
//			motor.motorStop = 0;  //������� �� 
//			setPWM = 100; 
//			hall=GPIO_ReadInputData(GPIOA);
//			hall=hall&0x0007; //0000 0001 1100 0000  // 0111 0000 0000 0000
//			if(motor.motorDirection)hall=7-hall;
//			Hall_SW();
//		}
//		if(hall == 5)
//			hall =1;
//		else if(hall == 1)
//				hall =3;
//		else if(hall == 3)
//				hall =2;
//		else if(hall == 2)
//				hall =6;
//		else if(hall == 6)
//				hall =4;
//		else if(hall == 4)
//				hall =5;
//		Delay_ms(20);
//		Hall_SW();
		
		if(hallc == 7)
		{
			motor.motorStop = 1;
			setPWM = 100;
			StopModelPWM();
			return;
		}
	}	
}

void StopModelPWM()
{
	if(motor.hallAvaiable == 1) //����������������
	{
		setPWM = 0;
		TIM1->CCR1=0;
		TIM1->CCR2=0; 		  
		TIM1->CCR3=0;
		GPIO_ResetBits(GPIOE, GPIO_Pin_8 | GPIO_Pin_10 | GPIO_Pin_12); 
	}
	else
	{
		if(motor.motorStop == 1)
		{
			if(TIM1->CCR1!=0)
				TIM1->CCR1 = setPWM;		
			else if (TIM1->CCR2 != 0)
				TIM1->CCR2 = setPWM;
			else if (TIM1->CCR3 != 0)
				TIM1->CCR3 = setPWM;		
		}
	}
}



















