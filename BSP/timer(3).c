#include "main.h"

//Timer 2 32-bit counter  
//Timer Clock is 168MHz / 4 * 2 = 84M
//APB1 84M
//APB2 168M


/******************************************************************************
***
* FUNCTION NAME:TIM1_Configuration
* CREATE DATE :2017/10/25
* CREATED BY :»Æç÷·ã
* FUNCTION :½«TIM1ÅäÖÃ³É²¦µ¯µç»úµÄ±àÂëÆ÷Ä£Ê½¡£
* MODIFY DATE :2017/10/25
* INPUT :ÎÞ
* OUTPUT :ÎÞ
* RETURN :ÎÞ
*******************************************************************************
**/
void TIM1_Configuration()//µç»ú±àÂëÆ÷
{
		GPIO_InitTypeDef GPIO_InitStructure;
	  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	  TIM_ICInitTypeDef TIM_ICInitStructure;   	

	
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
		GPIO_StructInit(&GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;         
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	  GPIO_Init(GPIOA, &GPIO_InitStructure);  
	
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource8,GPIO_AF_TIM1);
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_TIM1);
		
		
		TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
		TIM_TimeBaseStructure.TIM_Period = 2000; 
		TIM_TimeBaseStructure.TIM_Prescaler = 0; 
		TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1 ;	
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
		TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);               
		
		TIM_EncoderInterfaceConfig(TIM1, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising ,TIM_ICPolarity_Rising);
		TIM_ICStructInit(&TIM_ICInitStructure);
		TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICFilter = 100;
		
    TIM_ICInit(TIM1, &TIM_ICInitStructure);

		TIM_Cmd(TIM1, ENABLE);
		TIM1->CNT=2000+300-MotorAngle;	 
    TIM_ITConfig(TIM1, TIM_IT_Update,ENABLE);
    TIM_ClearFlag(TIM1, TIM_FLAG_Update);	
	
}

/******************************************************************************
***
* FUNCTION NAME:TIM10_Configuration
* CREATE DATE :2017/10/25
* CREATED BY :»Æç÷·ã
* FUNCTION :²úÉúÖÐ¶ÏÓÃÓÚ¼ì²â²¦µ¯µç»úÊÇ·ñ±»¿¨×¡¡£
* MODIFY DATE :2017/10/25
* INPUT :ÎÞ
* OUTPUT :ÎÞ
* RETURN :ÎÞ
*******************************************************************************
**/
void TIM10_Configuration(void)//É¨Ãè²¦µ¯µç»ú
{
    TIM_TimeBaseInitTypeDef  tim;
    NVIC_InitTypeDef         nvic;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10,ENABLE);
    
    nvic.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 1;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);

    tim.TIM_Prescaler = 16800-1;      
    tim.TIM_CounterMode = TIM_CounterMode_Up;
    tim.TIM_ClockDivision = TIM_CKD_DIV1;
    tim.TIM_Period = 500;  
    TIM_TimeBaseInit(TIM10,&tim);
		
    TIM_Cmd(TIM10, ENABLE);	 
	
    TIM_ITConfig(TIM10, TIM_IT_Update,ENABLE);
	
    TIM_ClearFlag(TIM10, TIM_FLAG_Update);	
		
}

extern ProjectilaMotor MotorState;

/******************************************************************************
***
* FUNCTION NAME:TIM1_UP_TIM10_IRQHandler
* CREATE DATE :2017/10/25
* CREATED BY :»Æç÷·ã
* FUNCTION :TIM1ºÍTIM10µÄÖÐ¶Ï´¦Àíº¯Êý£¬ÊµÏÖµç»ú·´×ª¡¢¶¨½Ç¹¦ÄÜ
* MODIFY DATE :2017/10/25
* INPUT :ÎÞ
* OUTPUT :ÎÞ
* RETURN :ÎÞ
*******************************************************************************
**/
void TIM1_UP_TIM10_IRQHandler()
{
//	printf("%d\r\n",TIM1->CNT);
	static int timecnt=0;
	static int timecntnow=0;
	if (TIM_GetITStatus(TIM10,TIM_IT_Update)!= RESET)
		{
			if(MotorState!=Stop)
			{
					
					if(MotorState==FirstKeepRunning||MotorState==FirstRunning)
					{
						timecnt=TIM1->CNT;
						MotorState=(MotorState==FirstKeepRunning)?KeepRunning:Running;
					}
					else 
					{
						timecntnow=TIM1->CNT;
						if((timecntnow-timecnt)<=20&&((timecntnow-timecnt)>=-20))
						{
							GPIO_ToggleBits(GPIOA,GPIO_Pin_2);
							GPIO_ToggleBits(GPIOA,GPIO_Pin_3);
							if(MotorState==Running)
							{
								TIM1->CNT=300;
							}
						}
						timecnt=timecntnow;
			    }
		  }
			TIM_ClearITPendingBit(TIM10,TIM_IT_Update);
			TIM_ClearFlag(TIM10, TIM_FLAG_Update);
		}
	   
		if (TIM_GetITStatus(TIM1,TIM_IT_Update)!= RESET)
		{
			if(MotorState==Running||MotorState==FirstRunning)
			{
				StopShooting();		
				//if(TIM1->CNT>1200)TIM1->CNT=(2000+2000-MotorAngle)-TIM1->CNT;
				if(TIM1->CNT>1200)TIM1->CNT=(2000+400-MotorAngle);
				else TIM1->CNT+=(2000-MotorAngle);
				GPIO_ToggleBits(GPIOB,GPIO_Pin_9);
			}	
			  TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
        TIM_ClearFlag(TIM1, TIM_FLAG_Update);
			
		}
}
