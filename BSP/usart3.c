#include "main.h"
#include "FIFO.h"
#include "protocal.h"

/*-----USART3_TX-----PB10-----*/
/*-----USART3_RX-----PB11-----*/

FIFO_S_t* UART_TranFifo;
static unsigned char rx_buffer[256];
uint8_t receive_from_pi = 0;
uint8_t remote_mode;
extern int now[4];
extern int add[4];
extern int tmp[4];
void USART3_Configuration(void)
{
	// todo 串口通信
    USART_InitTypeDef usart3;
    GPIO_InitTypeDef  gpio;
    NVIC_InitTypeDef  nvic;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);

    GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); 

    gpio.GPIO_Pin =  GPIO_Pin_10 | GPIO_Pin_11;
    gpio.GPIO_Mode = GPIO_Mode_AF;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
    gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;//GPIO_PuPd_UP;
    GPIO_Init(GPIOB,&gpio);

    usart3.USART_BaudRate = 115200;          // speed 10byte/ms
    usart3.USART_WordLength = USART_WordLength_8b;
    usart3.USART_StopBits = USART_StopBits_1;
    usart3.USART_Parity = USART_Parity_No;
    usart3.USART_Mode = USART_Mode_Tx	| USART_Mode_Rx;
    usart3.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART3,&usart3);

    USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
    USART_Cmd(USART3,ENABLE);

    nvic.NVIC_IRQChannel = USART3_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority = 0;
    nvic.NVIC_IRQChannelCmd = ENABLE; 
    NVIC_Init(&nvic);

    UART_TranFifo = FIFO_S_Create(100);  
    if(!UART_TranFifo)
    {
       // while(1);  avoid while in program
	}
}

uint32_t res(uint32_t num)
{
		uint32_t tmp=0;
    while(num){
				tmp=tmp*10+num%10;
			   num/=10;
		}
		 return tmp;
}
void UART3_Printint(uint32_t num)
{    
	 // num=res(num);
	  while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET){}
	  if(num!=0){
				while(num){
						USART_SendData(USART3, num%10+'0');
						while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET){}
						num/=10;
				}
		}
		else
			 USART_SendData(USART3,'0');
		while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET){}
}

void UART3_PrintCh(uint8_t ch)
{    
    FIFO_S_Put(UART_TranFifo, ch);
    USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
}

void UART3_PutStr (const char *str)//发送字符串
{
    while(*str)
    {
			  USART_SendData(USART3, *str++); 
			  while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET){}
    }
}

void UART3_PrintBlock(uint8_t* pdata, uint8_t len)
{
	uint8_t i = 0;
    for(i = 0; i < len; i++)
    {
        FIFO_S_Put(UART_TranFifo, pdata[i]);
    }
    USART_ITConfig(USART3, USART_IT_TXE, ENABLE);  //发送寄存器空中断
}


int fputc(int ch, FILE *f)
{
    while (USART_GetFlagStatus(USART3,USART_FLAG_TC) == RESET);
    USART_SendData(USART3, (uint8_t)ch);
    return ch;
}

void USART3_IRQHandler(void)
{  
    if(USART_GetITStatus(USART3, USART_IT_TXE) != RESET)
    {   
        if(!FIFO_S_IsEmpty(UART_TranFifo))
        {
					 uint16_t data = (uint16_t)FIFO_S_Get(UART_TranFifo);
					 USART_SendData(USART3, data);
        }
        else
        {
					 USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
        }  
    }
		else if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)    //接收中断
    {
				remote_mode=USART_ReceiveData(USART3);
			  USART_SendData(USART3,'A');
			/*if(data=='1')
				{	if(WriteFlash(0x080E0000,data))
						 data=ReadFlash(0x080E0000);
				}
				else
					data=ReadFlash(0x080E0000)+1;
				USART_SendData(USART3,data);
			
			for(int i=0;i<4;++i)
				UART3_Printint(tmp[i]),USART_SendData(USART3,'A');*/
    }     			
}

uint8_t get_mode(){
	return remote_mode;
}
