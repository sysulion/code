#include "main.h"

void BSP_Init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);     
	PWM_Configuration();           
	Laser_Configuration();
	TIM2_Configuration();		   
	TIM6_Configuration();			
	CAN1_Configuration();           
	CAN2_Configuration();            
	USART1_Configuration(100000);        
	USART3_Configuration();         
	TIM6_Start();   	 
  super_Configuration() ;	
	//Encoder_Start();
}

