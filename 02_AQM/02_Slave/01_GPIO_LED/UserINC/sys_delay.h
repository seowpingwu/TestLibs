#ifndef __SYS_DELAY_H_
#define __SYS_DELAY_H_

#include <stm32f10x_conf.h>

void Systick_Config(u8 SysClk); 	 //初始化函数
void Systick_Delayms(u16 dly); 		 //延时毫秒函数
void Systick_Delayus(u32 dly); 		 //延时微秒函数



#endif /*sys_delay.h*/
