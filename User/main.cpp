#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_exti.h"
#include "misc.h" 
#include "stm32f4xx_syscfg.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_usart.h"
char t;


/*
	GPIO: 1.Khai bao tan so cho PortX(X:A, B, C, D,...)
				2.Tao bien cau truc cho chan GPIO
				3.Khai bao cau truc cho bien vua tao
				4.Them bien cau truc vao PortX GPIOX
*/
	void Init_GPIO() //OUTPUT And OUTPUT
	{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE); //khai bao Clock cho GPIOG
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); //Khai bao Clock cho GPIOA
		//GPIO_DeInit GPIOG; //
		GPIO_InitTypeDef PINO;//Struct for GPIO_InitStructure OUTPUT
		PINO.GPIO_Pin = GPIO_Pin_14;//Define GPIO_InitStructure //select Pin of Port
		PINO.GPIO_Mode = GPIO_Mode_OUT;//select mode for Pin of Port
		PINO.GPIO_PuPd = GPIO_PuPd_NOPULL;//select status Pin pull-up or pull-down or non
		PINO.GPIO_Speed = GPIO_High_Speed;// select speed gpio
		PINO.GPIO_OType = GPIO_OType_PP;// select source DC
		GPIO_Init(GPIOG, &PINO);// add PINO into GPIOG

		GPIO_InitTypeDef PINI;//Struct for GPIO_InitStructure INPUT
		PINI.GPIO_Pin = GPIO_Pin_0;//Define GPIO_InitStructure
		PINI.GPIO_Mode = GPIO_Mode_IN;
		PINI.GPIO_PuPd = GPIO_PuPd_DOWN;
		PINI.GPIO_Speed = GPIO_High_Speed;
		PINI.GPIO_OType = GPIO_OType_OD;
		GPIO_Init(GPIOA, &PINI);
	}

static void Init_Interrupt() //EXTERNAL INTERRUPT
{
	
/*
	Interrupt:1.InitTypeDef cho EXTI,GPIO,NVIC
								1.1: EXTI dinh nghia cho external interrupt
								1.2: GPIO dinh nghia chi ngo vai interrupt
								1.3: NVIC chon kenh ngat va che do uu tien cho kenh ngat
*/
	
	//set value use
		EXTI_InitTypeDef EXTI_InitStructure; //define external interrupt
		GPIO_InitTypeDef GPIO_InitStructure; //define GPIO input for external interrupt
		NVIC_InitTypeDef NVIC_InitStructure; //Nested vector interrupt controller
		
		//CLOCK GPIO_InitStructure Interrput
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		//clock for SYSCFG
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	
		
		//SET GPIO_InitStructure INPUT
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
		GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		
			//Tell sys PA0 will use EXTI_Line0
		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
		
		//connect PA0 with EXTI_Line0
		EXTI_InitStructure.EXTI_Line = EXTI_Line0;
		//Enable interrupt
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		//Interrupt Mode
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		//Triggers on falling	edge
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//////////////////////////////////////////////////////
		//add EXTI_InitStructure EXTI
		EXTI_Init(&EXTI_InitStructure);
		
		
		//add IRQ vector to NVIC
		//PA0 is connected to EXTI_Line0, which has EXTI0_IRQn vector
		NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
		//Set priority
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		//Set sub priority
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		//Enable Interrupt
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		//add to NVIC
		NVIC_Init(&NVIC_InitStructure);
		
		///////////////////////////////////////////////////////////////////////////////////////////// define GPIO
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
		GPIO_InitTypeDef PINO;//Struct for GPIO_InitStructure OUTPUT
		PINO.GPIO_Pin = GPIO_Pin_13;//Define GPIO_InitStructure
		PINO.GPIO_Mode = GPIO_Mode_OUT;//
		PINO.GPIO_PuPd = GPIO_PuPd_NOPULL;
		PINO.GPIO_Speed = GPIO_High_Speed;
		PINO.GPIO_OType = GPIO_OType_PP;
		GPIO_Init(GPIOG, &PINO);
}

void Init_Timer()
{
	//init
	TIM_TimeBaseInitTypeDef TIM_BaseStruct;
	//define clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	//define timer
	TIM_BaseStruct.TIM_Prescaler = 41999; //1Khz = 42Mhz / (TIM_Prescaler + 1)
	TIM_BaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_BaseStruct.TIM_Period = 500; //TOP counter 0.5second
	TIM_BaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_BaseStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4, &TIM_BaseStruct);
	TIM_Cmd(TIM4, ENABLE);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	
		NVIC_InitTypeDef NVIC_InitStructure;
		NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn ;
		//Set priority
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
		//Set sub priority
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
		//Enable Interrupt
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		//add to NVIC
		NVIC_Init(&NVIC_InitStructure);
		
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
		GPIO_InitTypeDef PINO;//Struct for GPIO_InitStructure OUTPUT
		PINO.GPIO_Pin = GPIO_Pin_14;//Define GPIO_InitStructure
		PINO.GPIO_Mode = GPIO_Mode_OUT;//
		PINO.GPIO_PuPd = GPIO_PuPd_NOPULL;
		PINO.GPIO_Speed = GPIO_High_Speed;
		PINO.GPIO_OType = GPIO_OType_PP;
		GPIO_Init(GPIOG, &PINO);
}

void TIM_PWM_Configuration(void)
{ 
		GPIO_InitTypeDef GPIO_InitStruct;
   
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
    
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource5, GPIO_AF_TIM1);
    
    /* Set pins */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOE, &GPIO_InitStruct);
	
		TIM_TimeBaseInitTypeDef TIM_BaseStruct;
    
   
    RCC_APB1PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    TIM_BaseStruct.TIM_Prescaler = 0;
   
    TIM_BaseStruct.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_BaseStruct.TIM_Period = 8399; /* 10kHz PWM */
    TIM_BaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_BaseStruct.TIM_RepetitionCounter = 0;
    
    TIM_TimeBaseInit(TIM1, &TIM_BaseStruct);
   
    TIM_Cmd(TIM1, ENABLE);
		
		TIM_OCInitTypeDef TIM_OCStruct;
    
    
    TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_Low;
    

    TIM_OCStruct.TIM_Pulse = 2099;
    TIM_OC1Init(TIM1, &TIM_OCStruct);
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
}

void Init_Uart()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9 ,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10 ,GPIO_AF_USART1);
	
	USART_InitStructure.USART_BaudRate            = 115200;
	USART_InitStructure.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Parity							= USART_Parity_No;
	USART_InitStructure.USART_WordLength					= USART_WordLength_8b;
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	USART_Init(USART1,&USART_InitStructure);
	USART_Cmd(USART1, ENABLE);
}

int main()
{	
		Init_Interrupt();
		Init_Timer();
		Init_GPIO();
		Init_Uart();
		TIM_PWM_Configuration();
		while(1)
		{
//				t = USART_ReceiveData(USART1);
//				USART_ClearFlag(USART1, USART_FLAG_RXNE);
		}
}

extern "C" void EXTI0_IRQHandler(void)
{
		GPIO_ToggleBits(GPIOG, GPIO_Pin_13);
		EXTI_ClearITPendingBit(EXTI_Line0);
}


extern "C" void TIM4_IRQHandler(void)
{
		GPIO_ToggleBits(GPIOG, GPIO_Pin_14);
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
		USART_SendData(USART1, t);
}
