#ifndef __SYSLED_H_
#define __SYSLED_H_

#include "stm32f10x_conf.h"



#define  LED_Runing_PORT 					 GPIOA
#define  LED_Runing_Pin  					 GPIO_Pin_8

#define  LED_SensorDataErr_PORT 	 GPIOB
#define  LED_SensorDataErr_Pin     GPIO_Pin_0

#define  LED_NetWorkLinkErr_PORT 	 GPIOB
#define  LED_NetWorkLinkErr_Pin    GPIO_Pin_1



#define  LED_Runing_LOW  GPIO_ResetBits(LED_Runing_PORT,LED_Runing_Pin)  
#define  LED_Runing_High GPIO_SetBits(LED_Runing_PORT,LED_Runing_Pin)   

#define  LED_LED_SensorDataErr_LOW  	 GPIO_ResetBits(LED_SensorDataErr_PORT,LED_SensorDataErr_Pin)  
#define  LED_LED_SensorDataErr_High 	 GPIO_SetBits(LED_SensorDataErr_PORT,LED_SensorDataErr_Pin)   

#define  LED_LED_NetWorkLinkErr_LOW  	 GPIO_ResetBits(LED_NetWorkLinkErr_PORT,LED_NetWorkLinkErr_Pin)  
#define  LED_LED_NetWorkLinkErr_High 	 GPIO_SetBits(LED_NetWorkLinkErr_PORT,LED_NetWorkLinkErr_Pin)   



void SysRuningstatusLED_Init(void);

#endif
