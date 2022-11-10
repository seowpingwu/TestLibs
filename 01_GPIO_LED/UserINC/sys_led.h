#ifndef __SYS_LED_H_
#define __SYS_LED_H_

#include "stm32f10x_conf.h"

#define enDUGLED   	TURE  //
#define disDUGLED  	FALSE //

#define LEDDUGSTAT  enDUGLED



// R-ºìÉ«
#define  LED_Runing_PORT 					 GPIOB
#define  LED_Runing_Pin  					 GPIO_Pin_5

// G-ÂÌÉ«
#define  LED_SensorDataErr_PORT 	 GPIOB
#define  LED_SensorDataErr_Pin     GPIO_Pin_0

// B-À¶É«
#define  LED_NetWorkLinkErr_PORT 	 GPIOB
#define  LED_NetWorkLinkErr_Pin    GPIO_Pin_1



#define  LED_Runing_Low  GPIO_ResetBits(LED_Runing_PORT,LED_Runing_Pin)  
#define  LED_Runing_High GPIO_SetBits(LED_Runing_PORT,LED_Runing_Pin)   

#define  LED_LED_SensorDataErr_Low 	   GPIO_ResetBits(LED_SensorDataErr_PORT,LED_SensorDataErr_Pin)  
#define  LED_LED_SensorDataErr_High 	 GPIO_SetBits(LED_SensorDataErr_PORT,LED_SensorDataErr_Pin)   

#define  LED_LED_NetWorkLinkErr_Low  	 GPIO_ResetBits(LED_NetWorkLinkErr_PORT,LED_NetWorkLinkErr_Pin)  
#define  LED_LED_NetWorkLinkErr_High 	 GPIO_SetBits(LED_NetWorkLinkErr_PORT,LED_NetWorkLinkErr_Pin)  


void SysRuningStatus(void);
void SysSensorStatus(void);
void SysNetWorkStatus(void);


void Sys_LEDUserConfig(void);

#endif
