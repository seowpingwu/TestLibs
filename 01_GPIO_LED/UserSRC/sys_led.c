#include "stm32f10x_conf.h"

void Sys_LEDUserConfig(void){

	GPIO_InitTypeDef GPIO_InitStructure; //���ýṹ�����
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); //��������ʱ��GPIOB
	
	GPIO_InitStructure.GPIO_Pin = LED_Runing_Pin|LED_SensorDataErr_Pin|LED_NetWorkLinkErr_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;  //��������߿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //��������ʽ���ģʽ
	GPIO_Init(GPIOB,&GPIO_InitStructure);             //��ʼ��ָ������
	
		LED_Runing_High; 
		LED_LED_SensorDataErr_High;
	  LED_LED_NetWorkLinkErr_High;
	
	
}

