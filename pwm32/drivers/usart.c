#include "usart.h"
#include "include.h"
#include "motor.h"
#include "PID.h"

void Usart1_Init(u32 br_num)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);	
	
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
	
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_USART3);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOD, &GPIO_InitStructure); 

	//配置USART1
	USART_InitStructure.USART_BaudRate = br_num;       //波特率可以通过地面站配置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;  //8位数据
	USART_InitStructure.USART_StopBits = USART_StopBits_1;   //在帧结尾传输1个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;    //禁用奇偶校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //硬件流控制失能
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;  //发送、接收使能
	USART_Init(USART3, &USART_InitStructure);
	
	//使能USART1接收中断
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

	//使能USART1
	USART_Cmd(USART3, ENABLE); 

}

u8 recevedate[40] = {0} ;
u8 receveadd=0,receveok=0,count1 = 0;

void RecevedatePrepare(u8 date)
{
	static u8 findID = 0, r1=0,r2=0;
	
	r1 = r2;
	r2 = date;
	
	if(findID == 0)
	{
		if((r1==0x86)&&(r2==0x05))
		{
			findID=1;
		}else
		   findID=0;
	}
	else
	{
		recevedate[count1++] = date;	
		if(count1>=13)
		{
			receveadd = receveadd + 0x86 + 0x05;
			if(recevedate[12] == receveadd)
			{
				TIM_SetCounter(TIM5,0);
				receveok = 1;
				
				BYTE0(motor.shellLocation) = recevedate[0];
				BYTE1(motor.shellLocation) = recevedate[1];
				BYTE2(motor.shellLocation) = recevedate[2];
				BYTE3(motor.shellLocation) = recevedate[3];	
				BYTE0(motor.shellSpeed) = recevedate[4];
				BYTE1(motor.shellSpeed) = recevedate[5];
				BYTE2(motor.shellSpeed) = recevedate[6];
				BYTE3(motor.shellSpeed) = recevedate[7];	
				//motor.shellSpeed = 2160;
			}
			count1 = 0;
			receveadd = 0;
			findID = 0;
		}	
		else
		{
			receveadd += date;
		}
	}
}

u8 TxBuffer[256];
u8 TxCounter=0;
u8 count=0; 
u16 dtimer = 0;
void Usart1_IRQ(void)
{
	u8 com_data;
	
	if(USART3->SR & USART_SR_ORE)//ORE中断
	{
		com_data = USART3->DR;
	}

  //接收中断
	if( USART_GetITStatus(USART3,USART_IT_RXNE) )
	{
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);//清除中断标志

		com_data = USART3->DR;
		RecevedatePrepare(com_data);
	
	}
	//发送（进入移位）中断
	if( USART_GetITStatus(USART3,USART_IT_TXE ) )
	{
				
		USART3->DR = TxBuffer[TxCounter++]; //写DR清除中断标志          
		if(TxCounter == count)
		{
			USART3->CR1 &= ~USART_CR1_TXEIE;		//关闭TXE（发送中断）中断
		}
		//USART_ClearITPendingBit(USART2,USART_IT_TXE);
	}
}

void Usart1_Send(unsigned char *DataToSend ,u8 data_num)
{
	u8 i;
	for(i=0;i<data_num;i++)
	{
		TxBuffer[count++] = *(DataToSend+i);
	}

	if(!(USART3->CR1 & USART_CR1_TXEIE))
	{
		USART_ITConfig(USART3, USART_IT_TXE, ENABLE); //打开发送中断
	}
}

float lc=0;
float lllc=0;

u8 sendd[15] = {0x86,0x05};
u16 cou = 0,t;
u8 add=0;
void SendToFC()//绝对 相对 速度
{
	//TIM_SetCounter(TIM5,0);
	add=0;
	lc = 0;//motor.shellLocation; //motor.motorLocation;
	sendd[2] = BYTE0(lc);
	sendd[3] = BYTE1(lc);
	sendd[4] = BYTE2(lc);
	sendd[5] = BYTE3(lc);
	
	lc = motor.motorSpeed;//motor.motorSpeed;//motor.motorSpeed; //motor.motorSpeed;
	//LIMIT(lc,1,8888);
	sendd[6] = BYTE0(lc);
	sendd[7] = BYTE1(lc);
	sendd[8] = BYTE2(lc);
	sendd[9] = BYTE3(lc);
	
	lc = motor.shellSpeed;//motor.shellSpeed;// motor.motorSpeed;//motor.shellSpeed; 
	//LIMIT(lc,1,8888);
	sendd[10] = BYTE0(lc);
	sendd[11] = BYTE1(lc);
	sendd[12] = BYTE2(lc);
	sendd[13] = BYTE3(lc);

	for(t=0;t<14;t++)
	{
		add += sendd[t];
	}
	sendd[14] = add;

	for(t=0;t<15;t++)
	{
		USART_SendData(USART3, sendd[t]);         
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);//等待发送结束
	}
	//dtimer = TIM_GetCounter(TIM5);
}





