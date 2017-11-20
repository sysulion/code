#include "main.h"
#include "FIFO.h"
#include "protocal.h"
#include "flashrw.h"
#include "mpu6050_driver.h"
/*-----USART3_TX-----PB10-----*/
/*-----USART3_RX-----PB11-----*/

FIFO_S_t* UART_TranFifo2;
//static unsigned char rx_buffer[256];
extern int receive[3];
//uint8_t receive_from_pi = 0;
//uint8_t rx_count=0;
//uint8_t remote_mode;
extern int aim[4];
extern int add[4];
extern int tmp[4];
//float a[3],w[3],angle[3],T,tmp_angle[3];
uint8_t Re_buf[11];
uint8_t counter;
//static unsigned char sign;
void USART2_Configuration(void)
{
	// todo 串口通信
    USART_InitTypeDef usart2;
    GPIO_InitTypeDef  gpio;
    NVIC_InitTypeDef  nvic;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);

    GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_USART2);
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource6,GPIO_AF_USART2); 

    gpio.GPIO_Pin =  GPIO_Pin_5 | GPIO_Pin_6;
    gpio.GPIO_Mode = GPIO_Mode_AF;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
    gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;//GPIO_PuPd_UP;
    GPIO_Init(GPIOD,&gpio);

    usart2.USART_BaudRate =115200;          // speed 10byte/ms
    usart2.USART_WordLength = USART_WordLength_8b;
    usart2.USART_StopBits = USART_StopBits_1;
    usart2.USART_Parity = USART_Parity_No;
    usart2.USART_Mode = USART_Mode_Tx	| USART_Mode_Rx;
    usart2.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART2,&usart2);

    USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
    USART_Cmd(USART2,ENABLE);

    nvic.NVIC_IRQChannel = USART2_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 1;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE; 
    NVIC_Init(&nvic);

    UART_TranFifo2 = FIFO_S_Create(100);  
    if(!UART_TranFifo2)
    {
       // while(1);  avoid while in program
	}
}

void UART2_Printint(uint32_t num)
{    
	 // num=res(num);
	  while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET){}
	  if(num!=0){
				while(num){
						USART_SendData(USART2, num%10+'0');
						while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET){}
						num/=10;
				}
		}
		else
			 USART_SendData(USART2,'0');
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET){}
}

void UART2_PrintCh(uint8_t ch)
{    
    FIFO_S_Put(UART_TranFifo2, ch);
    USART_ITConfig(USART2,USART_IT_TXE, ENABLE);
}

void UART2_PutStr (const char *str)//发送字符串
{
    while(*str)
    {
			  USART_SendData(USART2, *str++); 
			  while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET){}
    }
}

void UART2_PrintBlock(uint8_t* pdata, uint8_t len)
{
	uint8_t i = 0;
    for(i = 0; i < len; i++)
    {
        FIFO_S_Put(UART_TranFifo2, pdata[i]);
    }
    USART_ITConfig(USART2, USART_IT_TXE, ENABLE);  //发送寄存器空中断
}


int MPU6050_Readdone=0;   //数据包读取完成标志
float a,b,c,d,e,f1,g1,h,i; //各数据返回值
float last_a,last_b;       
float Speed_x=0,Speed_y=0;
float Displacement_x=0,Displacement_y=0;
void USART2_IRQHandler(void)
{  
    if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET)
    {   
        if(!FIFO_S_IsEmpty(UART_TranFifo2))
        {
					 uint16_t data = (uint16_t)FIFO_S_Get(UART_TranFifo2);
					 USART_SendData(USART2, data);
        }
        else
        {
					 USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
        }  
    }
		else if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET&&MPU6050_Readdone==0)    //接收中断
    {
			   uint8_t data_get;
			   data_get=USART_ReceiveData(USART2);
			//if(MPU6050_Readdone==0)
			//{
			   if(data_get==0x55&&counter==0)//识别数据包头帧0X55
				 {
					 Re_buf[counter]=data_get;
					 counter++;
				 }
				 else
				 {	 
				 if(counter>0)
				 {
					 Re_buf[counter]=data_get;
					 counter++;
					 if(counter==11) //数据包阅读结束
				  {
						counter=0;
						MPU6050_Readdone=1;
						MPU6050_getDisplacement(&Displacement_x,&Displacement_y,&Speed_x,&Speed_y);//重置积分
						last_a = a;
						last_b = b;
						MPU6050_getMotion(&a,&b,&c,&d,&e,&f1,&g1,&h,&i);//处理加速度
						MPU6050_getSpeed(&Speed_x,&Speed_y,&a,&b,&last_a,&last_b);//积分处理
						
					 
					 //printf("%d,%d,%d,%d,%d,%d,\n",Re_buf[0],Re_buf[1],Re_buf[2],Re_buf[3],Re_buf[4],Re_buf[5]);
				  }
				 }
			 }
			// }	 
				//remote_mode=USART_ReceiveData(USART3);     			
			 
			/*if(data=='1')
				{	if(WriteFlash(0x080E0000,data))
						 data=ReadFlash(0x080E0000);
				}
				else
					data=ReadFlash(0x080E0000)+1;
				USART_SendData(USART3,data);
			
			for(int i=0;i<4;++i)
				UART3_Printint(tmp[i]),USART_SendData(USART3,'\t');*/
    }     			
}

