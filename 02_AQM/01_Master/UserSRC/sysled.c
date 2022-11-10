#include "stm32f10x_conf.h"

void SysRuningstatusLED_Init(void){

  GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);
	
//PA8
	GPIO_InitStructure.GPIO_Pin = LED_Runing_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	//PB0-PB0 SD&NL LED
	GPIO_InitStructure.GPIO_Pin = LED_SensorDataErr_Pin|LED_NetWorkLinkErr_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init( GPIOB,&GPIO_InitStructure);
  
	LED_LED_SensorDataErr_LOW ;
	LED_LED_NetWorkLinkErr_LOW;
	
	
 printf("LED 指示灯            配置完成\r");  
  
}

