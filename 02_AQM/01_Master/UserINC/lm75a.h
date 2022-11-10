#ifndef _LM75A_H_
#define _LM75A_H_

#include "stm32f10x_conf.h"

void LM75A_Write_Byte(u8 addr);
void LM75A_Read_Byte(u8 addr);
void LM75A_Read_Temp(float *dat);

#endif
