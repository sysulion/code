#include "main.h"
uint32_t Upload_Speed = 1;   //数据上传速度  单位 Hz
#define upload_time (1000000/Upload_Speed)  //计算上传的时间。单位为us
uint16_t  power1=0,power2=0;
uint32_t system_micrsecond;   //系统时间 单位ms
int main(void)
{     			
	delay_ms(800); 
  //CAN1_Configuration();                    
	USART1_Configuration(100000);        
	//USART3_Configuration();     			
	while(1)
	{   
//	  char temp[50];
//		sprintf(temp,"%d",32);	
//   	UART3_PutStr (temp);//发送字符串
  }
}
