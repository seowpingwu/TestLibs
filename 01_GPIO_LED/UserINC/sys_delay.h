#ifndef __SYS_DELAY_H_
#define __SYS_DELAY_H_

#include <stm32f10x_conf.h>

void Systick_Config(u8 SysClk); 	 //��ʼ������
void Systick_Delayms(u16 dly); 		 //��ʱ���뺯��
void Systick_Delayus(u32 dly); 		 //��ʱ΢�뺯��



#endif /*sys_delay.h*/
