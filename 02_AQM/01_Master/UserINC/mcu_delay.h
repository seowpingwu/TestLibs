#ifndef __MCU_DELAY_H_
#define __MCU_DELAY_H_

#include <stm32f10x_conf.h>

void Delay_Init(u8 Mhz); 				 //��ʼ������
void __delay_ms(u16 dly);  				 //��ʱ���뺯��
void __delay_us(u32 dly); //��ʱ΢�뺯��



#endif /*mcu_delay.h*/
