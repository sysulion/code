#include "main.h"
uint32_t Upload_Speed = 1;   //数据上传速度  单位 Hz
#define upload_time (1000000/Upload_Speed)  //计算上传的时间。单位为us
uint16_t  power1=0,power2=0;
uint32_t system_micrsecond;   //系统时间 单位ms
int main(void)
{     			
	delay_ms(800); 
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	Led_Configuration();
	USART1_Configuration(100000);        
	USART3_Configuration();
	PWM_Configuration();
	TIM6_Configuration();
	CAN1_Configuration();
	CAN2_Configuration();
	printf("2222");
	TIM6_Start();
	while(1){	delay_ms(3000); 
						control1(360);
		        while(1);
						delay_ms(3000); 
						control1(360);
		delay_ms(3000); 
						control1(-720);
		delay_ms(3000); 
						control1(-360);
		delay_ms(3000); 
						control1(360);
						delay_ms(3600); 
						control1(-180);
	};
	while(1){
		Set_Gimbal_Current(CAN2,500,0);
	//Set_CM_Speed(CAN1,1000,0,0,0);
	delay_ms(1000);
	Set_Gimbal_Current(CAN2,-500,0);delay_ms(1000);
		//Set_Gimbal_Current(CAN2,0,0);delay_ms(2000);
	}
	USART1_Configuration(100000);        
	//USART3_Configuration();
  
	while(1)
	{   
//	  char temp[50];
//		sprintf(temp,"%d",32);	
//   	UART3_PutStr (temp);//发送字符串
  }
}
