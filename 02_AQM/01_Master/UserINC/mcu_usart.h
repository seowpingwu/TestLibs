#ifndef __MCU_USART_H_
#define __MCU_USART_H_

#include <stm32f10x_conf.h>

#define TXD GPIO_Pin_9
#define RXD GPIO_Pin_10
#define USART_Prot GPIOA


void USARTX_UserConfig(u32 BaudRate);








#endif /*__MCU_USART_H_*/
