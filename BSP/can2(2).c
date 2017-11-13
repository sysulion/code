#include "main.h"

/*----CAN2_TX-----PB13----*/
/*----CAN2_RX-----PB12----*/

int16_t  pitch_ecd_bias =6000;
int16_t  yaw_ecd_bias  = 5000;

void CAN2_Configuration(void)
{/* //todo CAN总线协议 可做可不做
    CAN_InitTypeDef        can;
    CAN_FilterInitTypeDef  can_filter;
    GPIO_InitTypeDef       gpio;
    NVIC_InitTypeDef       nvic;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_CAN2);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_CAN2); 

    gpio.GPIO_Pin =  ;
    gpio.GPIO_Mode = ;
    GPIO_Init(,);

    nvic.NVIC_IRQChannel = ;
    nvic.NVIC_IRQChannelPreemptionPriority = ;
    nvic.NVIC_IRQChannelSubPriority =;
    nvic.NVIC_IRQChannelCmd = ;
    NVIC_Init(&nvic);
    
    nvic.NVIC_IRQChannel = ;
    nvic.NVIC_IRQChannelPreemptionPriority = ;
    nvic.NVIC_IRQChannelSubPriority = ;
    nvic.NVIC_IRQChannelCmd = ;
    NVIC_Init(&nvic);

    CAN_DeInit();
    CAN_StructInit();

    can.CAN_TTCM = ;
    can.CAN_ABOM = ;    
    can.CAN_AWUM = ;    
    can.CAN_NART = ;    
    can.CAN_RFLM = ;    
    can.CAN_TXFP = ;     
    can.CAN_Mode = ; 
    can.CAN_SJW  = ;
    can.CAN_BS1 = ;
    can.CAN_BS2 = ;
    can.CAN_Prescaler = ;   //CAN BaudRate 42/(1+9+4)/3=1Mbps
    CAN_Init(CAN2, &can);
    
    can_filter.CAN_FilterNumber=14;
    can_filter.CAN_FilterMode=CAN_FilterMode_IdMask;
    can_filter.CAN_FilterScale=CAN_FilterScale_32bit;
    can_filter.CAN_FilterIdHigh=0x0000;
    can_filter.CAN_FilterIdLow=0x0000;
    can_filter.CAN_FilterMaskIdHigh=0x0000;
    can_filter.CAN_FilterMaskIdLow=0x0000;
    can_filter.CAN_FilterFIFOAssignment=0;//the message which pass the filter save in fifo0
    can_filter.CAN_FilterActivation=ENABLE;
    CAN_FilterInit(&can_filter);
    
    CAN_ITConfig(CAN2,CAN_IT_FMP0,ENABLE);
    CAN_ITConfig(CAN2,CAN_IT_TME,ENABLE);
		*/
}

void CAN2_TX_IRQHandler(void) //CAN TX
{
  if (CAN_GetITStatus(CAN2,CAN_IT_TME)!= RESET)    //if transmit mailbox is empty 
  {
	   CAN_ClearITPendingBit(CAN2,CAN_IT_TME);   
  }
}


void CAN2_RX0_IRQHandler(void)
{
    CanRxMsg rx_message;
    if (CAN_GetITStatus(CAN2,CAN_IT_FMP0)!= RESET) 
    {
        CAN_ClearITPendingBit(CAN2, CAN_IT_FMP0);
        CAN_Receive(CAN2, CAN_FIFO0, &rx_message);  
       //电机编码器数据处理
       CanReceiveMsgProcess(&rx_message);
    }
}

