#include "stm32f10x_conf.h"

void Sys_LEDUserConfig(void){

	GPIO_InitTypeDef GPIO_InitStructure; //设置结构体变量
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); //开启外设时钟GPIOB
	
	GPIO_InitStructure.GPIO_Pin = LED_Runing_Pin|LED_SensorDataErr_Pin|LED_NetWorkLinkErr_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;  //设置输出线口速率
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //设置推挽式输出模式
	GPIO_Init(GPIOB,&GPIO_InitStructure);             //初始化指定分组
	
		LED_Runing_High; 
		LED_LED_SensorDataErr_High;
	  LED_LED_NetWorkLinkErr_High;
	
	
}

