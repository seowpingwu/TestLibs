#ifndef __MCU_I2C_H
#define	__MCU_I2C_H



#include "stm32f10x_conf.h"

#define SCL GPIO_Pin_6
#define SDA	GPIO_Pin_7

#define SCL_High	GPIO_SetBits(GPIOB,SCL)
#define SCL_Low		GPIO_ResetBits(GPIOB,SCL)

#define SDA_High	GPIO_SetBits(GPIOB,SDA)
#define SDA_Low		GPIO_ResetBits(GPIOB,SDA)



#define OUT 1
#define INPUT 0




void MCU_I2C_UserConfig(void);
void I2C_SDA_Mode(unsigned char addr);
void MCU_I2C_Start(void);
void MCU_I2C_Stop(void);
unsigned char MCU_I2C_Wait_Ack(void);
void MCU_I2C_Write(unsigned char  Data);
unsigned char  MCU_I2C_Read(void);
void MCU_I2C_Send_Ack(unsigned char ack);






#endif
