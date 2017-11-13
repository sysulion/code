#include "main.h"

void Laser_Configuration()  //¼¤¹â
{
	GPIO_InitTypeDef gpioInitStruct;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	gpioInitStruct.GPIO_Pin = GPIO_Pin_0;
	gpioInitStruct.GPIO_Mode = GPIO_Mode_OUT;
	gpioInitStruct.GPIO_OType = GPIO_OType_PP;
	gpioInitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOE, &gpioInitStruct);
	GPIO_ResetBits(GPIOE, GPIO_Pin_0);
}