#include "main.h"

/*----CAN1_TX-----PA12----*/
/*----CAN1_RX-----PA11----*/

void CAN1_Configuration(void)
{
	/*
		// todo CAN总线协议
    CAN_InitTypeDef        can;
    CAN_FilterInitTypeDef  can_filter;
    GPIO_InitTypeDef       gpio;
    NVIC_InitTypeDef       nvic;
    RCC_AHB1PeriphClockCmd(, );
    RCC_APB1PeriphClockCmd(, );

    GPIO_PinAFConfig(, , );
    GPIO_PinAFConfig(, , );

    gpio.GPIO_Pin =
    gpio.GPIO_Mode = 
    GPIO_Init(, );
    
    nvic.NVIC_IRQChannel = ;
    nvic.NVIC_IRQChannelPreemptionPriority = ;
    nvic.NVIC_IRQChannelSubPriority = ;
    nvic.NVIC_IRQChannelCmd = ;
    NVIC_Init(&nvic);
    
    nvic.NVIC_IRQChannel = ;
    nvic.NVIC_IRQChannelPreemptionPriority = ;
    nvic.NVIC_IRQChannelSubPriority = ;
    nvic.NVIC_IRQChannelCmd = ;
    NVIC_Init();    
    
    CAN_DeInit();
    CAN_StructInit();
    
    can.CAN_TTCM = ;
    can.CAN_ABOM = ;
    can.CAN_AWUM = ;
    can.CAN_NART = ;
    can.CAN_RFLM = ;
    can.CAN_TXFP = ;
    can.CAN_Mode = ;
    can.CAN_SJW = ;
    can.CAN_BS1 = ;
    can.CAN_BS2 = ;
    can.CAN_Prescaler = ;   //CAN BaudRate 42/(1+9+4)/3=1Mbps
    CAN_Init(, );    

	  can_filter.CAN_FilterNumber=0;
	  can_filter.CAN_FilterMode=CAN_FilterMode_IdMask;
	  can_filter.CAN_FilterScale=CAN_FilterScale_32bit;
	  can_filter.CAN_FilterIdHigh=0x0000;
	  can_filter.CAN_FilterIdLow=0x0000;
	  can_filter.CAN_FilterMaskIdHigh=0x0000;
	  can_filter.CAN_FilterMaskIdLow=0x0000;
	  can_filter.CAN_FilterFIFOAssignment=0;//the message which pass the filter save in fifo0
	  can_filter.CAN_FilterActivation=ENABLE;
	  CAN_FilterInit(&can_filter);
    
    CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);
    CAN_ITConfig(CAN1,CAN_IT_TME,ENABLE); 
		*/
}



void GYRO_RST(void)
{
    CanTxMsg tx_message;
    
    tx_message.StdId = 0x404;//send to gyro controll board
    tx_message.IDE = CAN_Id_Standard;
    tx_message.RTR = CAN_RTR_Data;
    tx_message.DLC = 0x08;
    
    tx_message.Data[0] = 0x00;
    tx_message.Data[1] = 0x01;
    tx_message.Data[2] = 0x02;
    tx_message.Data[3] = 0x03;
    tx_message.Data[4] = 0x04;
    tx_message.Data[5] = 0x05;
    tx_message.Data[6] = 0x06;
    tx_message.Data[7] = 0x07;
    
    CAN_Transmit(CAN1,&tx_message);
}


void CAN1_TX_IRQHandler(void) //CAN TX
{
    if (CAN_GetITStatus(CAN1,CAN_IT_TME)!= RESET) 
	{
		CAN_ClearITPendingBit(CAN1,CAN_IT_TME);
    }
}

/*************************************************************************
                          CAN1_RX0_IRQHandler
描述：CAN1的接收中断函数
*************************************************************************/
void CAN1_RX0_IRQHandler(void)
{   
	  CanRxMsg rx_message;	
    if (CAN_GetITStatus(CAN1,CAN_IT_FMP0)!= RESET)
	{
        CAN_ClearITPendingBit(CAN1, CAN_IT_FF0);
		    CAN_ClearFlag(CAN1, CAN_FLAG_FF0); 
		
		    CAN_Receive(CAN1, CAN_FIFO0, &rx_message);
			CanReceiveMsgProcess(&rx_message);
    }
}



