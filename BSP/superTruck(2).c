#include "stm32f4xx.h"
#include "superTruck.h"


void super_Configuration(void)
{
    GPIO_InitTypeDef gpio;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	  gpio.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_14| GPIO_Pin_13| GPIO_Pin_12;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
		gpio.GPIO_Mode = GPIO_Mode_OUT;
		gpio.GPIO_OType = GPIO_OType_PP;
		GPIO_Init(GPIOD,&gpio);
	  GPIO_ResetBits(GPIOD, GPIO_Pin_15);
	  GPIO_ResetBits(GPIOD, GPIO_Pin_14);
	  GPIO_ResetBits(GPIOD, GPIO_Pin_12);
	  GPIO_ResetBits(GPIOD, GPIO_Pin_11);
	  

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	  gpio.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_14;
	  gpio.GPIO_Mode = GPIO_Mode_IN;
	  gpio.GPIO_PuPd = GPIO_PuPd_UP;
	  GPIO_Init(GPIOB,&gpio);
	
}
