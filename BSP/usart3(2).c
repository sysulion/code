#include "main.h"
#include "FIFO.h"
#include "protocal.h"

/*-----USART3_TX-----PB10-----*/
/*-----USART3_RX-----PB11-----*/

FIFO_S_t* UART_TranFifo;
static unsigned char rx_buffer[256];
uint8_t receive_from_pi = 0;
void USART3_Configuration(void)
{
	/*
	// todo 串口通信
    USART_InitTypeDef usart3;
    GPIO_InitTypeDef  gpio;
    NVIC_InitTypeDef  nvic;

    RCC_AHB1PeriphClockCmd(,);
    RCC_APB1PeriphClockCmd(,);

    GPIO_PinAFConfig(,,);
    GPIO_PinAFConfig(,,); 

    gpio.GPIO_Pin =   ;
    gpio.GPIO_Mode = ;
    gpio.GPIO_OType = ;
    gpio.GPIO_Speed = ;
    gpio.GPIO_PuPd = ;
    GPIO_Init(,);

    usart3.USART_BaudRate = ;          // speed 10byte/ms
    usart3.USART_WordLength = ;
    usart3.USART_StopBits = ;
    usart3.USART_Parity = ;
    usart3.USART_Mode = ;
    usart3.USART_HardwareFlowControl = ;
    USART_Init(,);

    USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
    USART_Cmd(USART3,ENABLE);

    nvic.NVIC_IRQChannel = ;
    nvic.NVIC_IRQChannelPreemptionPriority = ;
    nvic.NVIC_IRQChannelSubPriority = ;
    nvic.NVIC_IRQChannelCmd = ; 
    NVIC_Init(&nvic);

    UART_TranFifo = FIFO_S_Create(100);  
    if(!UART_TranFifo)
    {
       // while(1);  avoid while in program
	}
	*/
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
    }else if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)    //接收中断
    {
    
    }       
}

