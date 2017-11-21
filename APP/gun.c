#include "stm32f4xx.h"
#include "gun.h"
/*-LEFT---(PA1---TIM5_CH2)--*/
/*-RIGHT--(PA0--TIM5_CH1)--*/

ProjectilaMotor MotorState=Stop;

/******************************************************************************
***
* FUNCTION NAME:Shoot_Configuration
* CREATE DATE :2017/10/25
* CREATED BY :»Æç÷·ã
* FUNCTION :³õÊ¼»¯Õû¸ö·¢Éä×°ÖÃ
* MODIFY DATE :2017/10/25
* INPUT :ÎÞ
* OUTPUT :ÎÞ
* RETURN :ÎÞ
*******************************************************************************
**/
void Shoot_Configuration(void)
{
	  PWM_Configuration();
	  ProjectileMotor_Config();
	  TIM1_Configuration();
	  TIM10_Configuration();
}

/******************************************************************************
***
* FUNCTION NAME:PWM_Configuration
* CREATE DATE :2017/10/25
* CREATED BY :»Æç÷·ã
* FUNCTION :³õÊ¼»¯Ä¦²ÁÂÖµÄPWM
* MODIFY DATE :2017/10/25
* INPUT :ÎÞ
* OUTPUT :ÎÞ
* RETURN :ÎÞ
*******************************************************************************
**/
void PWM_Configuration(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
	  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
		GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Pin   =  GPIO_Pin_0 ;
		GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_Init(GPIOA,&GPIO_InitStructure);
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM5);
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1 ;
		GPIO_Init(GPIOA,&GPIO_InitStructure);
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_TIM5);

		TIM_TimeBaseInitStructure.TIM_ClockDivision     = TIM_CKD_DIV1; 
		TIM_TimeBaseInitStructure.TIM_CounterMode       = TIM_CounterMode_Up;  
		TIM_TimeBaseInitStructure.TIM_Prescaler         = 84-1;  
		TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
		TIM_TimeBaseInitStructure.TIM_Period            = 10000;   
		TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);

		TIM_OCInitTypeDef TIM_OCInitStructure;
		TIM_OCInitStructure.TIM_OCMode 				= TIM_OCMode_PWM1;
		TIM_OCInitStructure.TIM_OutputState 	= TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_OutputNState  = TIM_OutputNState_Enable;
		TIM_OCInitStructure.TIM_Pulse 				= 800;
		TIM_OCInitStructure.TIM_OCPolarity 		= TIM_OCPolarity_High;
		TIM_OCInitStructure.TIM_OCNPolarity 	= TIM_OCPolarity_High;
		TIM_OCInitStructure.TIM_OCIdleState 	= TIM_OCIdleState_Set;
		TIM_OCInitStructure.TIM_OCNIdleState	= TIM_OCNIdleState_Reset;
		TIM_OC1Init(TIM5,&TIM_OCInitStructure);
		TIM_OC2Init(TIM5,&TIM_OCInitStructure);

		TIM_Cmd(TIM5,ENABLE);
		TIM_CtrlPWMOutputs(TIM5,ENABLE);
}

/******************************************************************************
***
* FUNCTION NAME:SetMotorSpeed
* CREATE DATE :2017/10/25
* CREATED BY :»Æç÷·ã
* FUNCTION :ÉèÖÃÄ¦²ÁÂÖµÄËÙ¶È
* MODIFY DATE :2017/10/25
* INPUT :´óÐ¡´¦ÓÚ1150~2000µÄÕýÕûÊý
* OUTPUT :ÎÞ
* RETURN :ÎÞ
*******************************************************************************
**/
void SetMotorSpeed(uint16_t uTimeCnt)
{
		TIM5->CCR1=uTimeCnt;
		TIM5->CCR2=uTimeCnt;
}

/******************************************************************************
***
* FUNCTION NAME:ProjectileMotor_Config
* CREATE DATE :2017/10/25
* CREATED BY :»Æç÷·ã
* FUNCTION :³õÊ¼»¯²¦µ¯µç»ú
* MODIFY DATE :2017/10/25
* INPUT :ÎÞ
* OUTPUT :ÎÞ
* RETURN :ÎÞ
*******************************************************************************
**/
void ProjectileMotor_Config(void)
{
		GPIO_InitTypeDef GPIO_InitStructure; 
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2|GPIO_Pin_3; 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;           
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
		GPIO_Init(GPIOA, &GPIO_InitStructure); 
}

/******************************************************************************
***
* FUNCTION NAME:ShootOneProjectile
* CREATE DATE :2017/10/25
* CREATED BY :»Æç÷·ã
* FUNCTION :´ò³öÒ»·¢×Óµ¯£¬´òÍêÒ»·¢×Óµ¯ºó²¦µ¯µç»úÍ£Ö¹
* MODIFY DATE :2017/10/25
* INPUT :ÎÞ
* OUTPUT :ÎÞ
* RETURN :ÎÞ
*******************************************************************************
**/
void ShootOneProjectile(void)
{
	
		//TIM1->ARR=500;
		GPIO_SetBits(GPIOA,GPIO_Pin_2);
		GPIO_ResetBits(GPIOA,GPIO_Pin_3);
		MotorState=FirstRunning;
	   
}

/******************************************************************************
***
* FUNCTION NAME:Shoot
* CREATE DATE :2017/10/25
* CREATED BY :»Æç÷·ã
* FUNCTION :Á¬ÐøµØ´ò³ö×Óµ¯£¬²»»á×Ô¶¯Í£Ö¹
* MODIFY DATE :2017/10/25
* INPUT :ÎÞ
* OUTPUT :ÎÞ
* RETURN :ÎÞ
*******************************************************************************
**/
void Shoot(void)
{
		GPIO_SetBits(GPIOA,GPIO_Pin_2);
		GPIO_ResetBits(GPIOA,GPIO_Pin_3);
		MotorState=FirstKeepRunning;
}

/******************************************************************************
***
* FUNCTION NAME:StopShooting
* CREATE DATE :2017/10/25
* CREATED BY :»Æç÷·ã
* FUNCTION :Í£Ö¹·¢Éä×Óµ¯
* MODIFY DATE :2017/10/25
* INPUT :ÎÞ
* OUTPUT :ÎÞ
* RETURN :ÎÞ
*******************************************************************************
**/
void StopShooting(void)
{
	  GPIO_ResetBits(GPIOA,GPIO_Pin_2|GPIO_Pin_3);
	  MotorState=Stop;
}
