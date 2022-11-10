#ifndef __MCU_DELAY_H_
#define __MCU_DELAY_H_

#include <stm32f10x_conf.h>

void Delay_Init(u8 Mhz); 				 //初始化函数
void __delay_ms(u16 dly);  				 //延时毫秒函数
void __delay_us(u32 dly); //延时微秒函数



#endif /*mcu_delay.h*/
